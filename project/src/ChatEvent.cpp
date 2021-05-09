#include <Wt/WServer.h>
#include "ChatEvent.hpp"

const Wt::WString ChatEvent::formattedHTML(const User &user,
                                           Wt::TextFormat format) const {
   /* switch (type_) {
        case Login:
            return Wt::WString("<span class='chat-info'>")
                   + Wt::WWebWidget::escapeText(user_.getUsername()) + " joined.</span>";
        case Logout:
            return Wt::WString("<span class='chat-info'>")
                   + ((user == user_) ?
                      Wt::WString("You") :
                      Wt::WWebWidget::escapeText(user_.getUsername()))
                   + " logged out.</span>";
        case UpdateProfile:
            return "<span class='chat-info'>"
                   + ((user.getUsername() == data_ || user == user_) ?
                      "You are" :
                      (Wt::WWebWidget::escapeText(user_.getUsername()) + " is"))
                   + " now known as " + Wt::WWebWidget::escapeText(data_) + ".</span>";
        case NewMessage: {
            Wt::WString result;

            result = Wt::WString("<span class='")
                     + ((user.getUsername() == message_.getFromUser()) ?
                        "chat-self" :
                        "chat-user")
                     + "'>" + Wt::WWebWidget::escapeText(message_.getFromUser()) + ":</span>";

            Wt::WString msg
                    = (format == Wt::TextFormat::XHTML ? message_.getMessage() : Wt::WWebWidget::escapeText(message_.getMessage()));

//            if (message_.toUTF8().find(user.getUsername().toUTF8()) != std::string::npos)
//                return result + "<span class='chat-highlight'>" + msg + "</span>";
//            else
//                return result + msg;
            return result + msg;
        }
        default:
            return "";
    }*/
    return Wt::WString();
}
