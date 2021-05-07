//
// Created by Борис Кожуро on 15.04.2021.
//
#include "InMemoryDb.hpp"

using Buf_t = tnt::Buffer<BUFSIZE>;
using BufIter_t = typename Buf_t::iterator;
using Network_t = NetworkEngine;
using Net_t = DefaultNetProvider<Buf_t, Network_t >;

struct UserTuple {
    std::string field1;
    std::string field2;
    uint32_t field3 = 0;
};

std::ostream& operator<<(std::ostream& strm, const UserTuple &t)
{
return strm << "Tuple: field1=" << t.field1 << " field2=" << t.field2 << " field3=" << t.field3;
}

//region READER
//region WORK
struct UserTupleValueReader : mpp::DefaultErrorHandler {
    bool first = true;
    explicit UserTupleValueReader(UserTuple& t) : tuple(t) {}
    static constexpr mpp::Type VALID_TYPES = mpp::MP_UINT | mpp::MP_STR;
    template <class T>
    void Value(const BufIter_t&, mpp::compact::Type, T v)
    {
        tuple.field3 = v;
    }
    void Value(const BufIter_t& itr, mpp::compact::Type, mpp::StrValue v)
    {
        BufIter_t tmp = itr;
        tmp += v.offset;
        std::string &dst = tuple.field1;
        while (v.size) {
            dst.push_back(*tmp);
            ++tmp;
            --v.size;
        }
        dst.push_back('\t');
    }
    void WrongType(mpp::Type expected, mpp::Type got)
    {
        std::cout << "expected type is " << expected <<
                  " but got " << got << std::endl;
    }

    BufIter_t* StoreEndIterator() { return nullptr; }
    UserTuple& tuple;
};

template <class BUFFER>
struct UserTupleReader : mpp::SimpleReaderBase<BUFFER, mpp::MP_ARR> {

    UserTupleReader(mpp::Dec<BUFFER>& d, UserTuple& t) : dec(d), tuple(t) {}

    void Value(const iterator_t<BUFFER>&, mpp::compact::Type, mpp::ArrValue u)
    {
        assert(u.size == 3);
        dec.SetReader(false, UserTupleValueReader{tuple});
    }
    mpp::Dec<BUFFER>& dec;
    UserTuple& tuple;
};

template <class BUFFER>
std::vector<UserTuple>
decodeUserTuple(BUFFER &buf, Data<BUFFER> &data)
{
    std::vector<UserTuple> results;
    for(auto const& t: data.tuples) {
        UserTuple tuple;
        mpp::Dec dec(buf);
        dec.SetPosition(t.begin);
        dec.SetReader(false, UserTupleReader<BUFFER>{dec, tuple});
        mpp::ReadResult_t res = dec.Read();
        assert(res == mpp::READ_SUCCESS);
        results.push_back(tuple);
    }
    return results;
}
//endregion
template<class BUFFER>
std::vector<UserTuple> printResponse(Connection<BUFFER, Net_t> &conn, Response<BUFFER> &response)
{
    if (response.body.error_stack != std::nullopt) {
        Error err = (*response.body.error_stack).error;
        std::cout << "RESPONSE ERROR: msg=" << err.msg <<
                  " line=" << err.file << " file=" << err.file <<
                  " errno=" << err.saved_errno <<
                  " type=" << err.type_name <<
                  " code=" << err.errcode << std::endl;
    }
    if (response.body.data != std::nullopt) {
        Data<BUFFER> data = *response.body.data;
        if (data.tuples.empty()) {
            std::cout << "Empty result" << std::endl;
            return std::vector<UserTuple> (0);
        }
        std::vector<UserTuple> tuples =
                decodeUserTuple(conn.getInBuf(), data);

        for (auto & t : tuples) {
            std::string delimiter = "\t";

            t.field2 = t.field1.substr( t.field1.find(delimiter) + 1, t.field1.size() - t.field1.find(delimiter)-2);
            t.field1 = t.field1.substr(0, t.field1.find(delimiter));
            if (t.field3 < 0)
                t.field3 = 0;
            std::cout << t << std::endl;
        }
        return tuples;
    }
    return std::vector<UserTuple> (0);
}
//endregion

int InMemoryDb::dbStart() {
    return 0;
}

int InMemoryDb::dbConnect(Connector<tnt::Buffer<BUFSIZE>, DefaultNetProvider<tnt::Buffer<BUFSIZE>, NetworkEngine>>&client, Connection<tnt::Buffer<BUFSIZE>, DefaultNetProvider<tnt::Buffer<BUFSIZE>, NetworkEngine>>&conn) {
    const char *address = "127.0.0.1";
    int port = 3301;
    int rc = client.connect(conn, address, port);
    if (rc != 0) {
        assert(conn.status.is_failed);
        std::cerr << conn.getError() << std::endl;
        return -1;
    }
    return 0;
}

std::string InMemoryDb::getUserName(std::string token) {
    Connector<Buf_t, Net_t> client;
    Connection<Buf_t, Net_t> conn(client);
    this->dbConnect(client, conn);
    uint32_t space_id = 600;
    std::string pk_value = token;
    uint32_t index_id = 1;
    uint32_t limit = 1;
    uint32_t offset = 0;
    IteratorType iter = IteratorType::EQ;
    auto i = conn.space[space_id].index[index_id];
    rid_t select = i.select(std::make_tuple(pk_value), limit, offset, iter);
    int WAIT_TIMEOUT = 1000; //milliseconds
    client.wait(conn, select);
    std::optional<Response<Buf_t>> response = conn.getResponse(select);
    assert(response != std::nullopt);
    std::vector<UserTuple> res = printResponse<Buf_t>(conn, *response);
    client.close(conn);
    if (res.empty()) {
        return "";
    }
    return res[0].field1;
}



int InMemoryDb::writeInMemory(std::string userName, std::string token, int status) {
    Connector<Buf_t, Net_t> client;
    Connection<Buf_t, Net_t> conn(client);
    this->dbConnect(client, conn);
    uint32_t space_id = 600;
    std::tuple data = std::make_tuple(userName, token, status);
    rid_t replace = conn.space[space_id].replace(data);
    int resp = client.wait(conn, replace);
    client.close(conn);
    if (resp == -1) {
        return -1;
    }
    return 0;
}

int InMemoryDb::updateUserStatus(std::string userName, int way) {
    // way= 1 ++ way =-1 --
    Connector<Buf_t, Net_t> client;
    Connection<Buf_t, Net_t> conn(client);
    this->dbConnect(client, conn);
    uint32_t space_id = 600;
    std::string pk_value = userName;
    uint32_t index_id = 0;
    uint32_t limit = 1;
    uint32_t offset = 0;
    IteratorType iter = IteratorType::EQ;
    auto i = conn.space[space_id].index[index_id];
    rid_t select = i.select(std::make_tuple(pk_value), limit, offset, iter);
    int WAIT_TIMEOUT = 1000; //milliseconds
    client.wait(conn, select);
    std::optional<Response<Buf_t>> response = conn.getResponse(select);
    assert(response != std::nullopt);
    std::vector<UserTuple> res = printResponse<Buf_t>(conn, *response);
    if (res.empty()) {
        return -1;
    }
    if (!(res[0].field3 == 0 && way == -1)) {
        res[0].field3 = res[0].field3 + way;
    }


    std::tuple data = std::make_tuple(res[0].field1, res[0].field2, res[0].field3);
    rid_t replace = conn.space[space_id].replace(data);
    int resp = client.wait(conn, replace);
    client.close(conn);
    if (resp == -1) {
        return -2;
    }
    return 0;
}

std::string InMemoryDb::searchToken(std::string token) {
    Connector<Buf_t, Net_t> client;
    Connection<Buf_t, Net_t> conn(client);
    this->dbConnect(client, conn);
    uint32_t space_id = 600;
    std::string pk_value = token;
    uint32_t index_id = 1;
    uint32_t limit = 1;
    uint32_t offset = 0;
    IteratorType iter = IteratorType::EQ;
    auto i = conn.space[space_id].index[index_id];
    rid_t select = i.select(std::make_tuple(pk_value), limit, offset, iter);
    int WAIT_TIMEOUT = 1000; //milliseconds
    client.wait(conn, select);
    std::optional<Response<Buf_t>> response = conn.getResponse(select);
    client.close(conn);
    if (response == std::nullopt) {
        return "";
    }
    std::vector<UserTuple> res = printResponse<Buf_t>(conn, *response);
    if (res.empty()) {
        return "";
    }
    return res[0].field1;
}

std::vector<bool> InMemoryDb::getOnline(std::vector<std::string> userVector) {
    Connector<Buf_t, Net_t> client;
    Connection<Buf_t, Net_t> conn(client);
    this->dbConnect(client, conn);
    uint32_t space_id = 600;
    uint32_t index_id = 0;
    uint32_t limit = 1;
    uint32_t offset = 0;
    auto j = conn.space[space_id].index[index_id];
    std::vector<bool> a(userVector.size(), false);
    IteratorType iter = IteratorType::EQ;
    for (int i = 0; i < userVector.size(); i++) {
        std::string pk_value = userVector[i];
        rid_t select = j.select(std::make_tuple(pk_value), limit, offset, iter);
        client.wait(conn, select);
        std::optional<Response<Buf_t>> response = conn.getResponse(select);
        std::vector<UserTuple> res = printResponse<Buf_t>(conn, *response);
        if (res[0].field3 > 0) {
            a[i] = true;
        }
    }
    client.close(conn);
    return  a;
}
