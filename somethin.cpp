#include <cpprestsdk/http_client.h>
#include <cpprestsdk/json.h>
#include <nlohmann/json.hpp>
#include "database.h"
#include "user.h"
#include "auth.h"
#include "jwt.h"

using namespace web::http;
using namespace nlohmann;

int main() {
}
  Database db("localhost", "your_database", "username", "password");

  http_listener listener("http://localhost:8080");
  listener.support(methods::POST, "/register", [](http_request request) {
    json::value body = request.extract_json().get();
    std::string email = body["email"];
    std::string password = body["password"];
    if (!regex_match(email, regex("[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,6}"))) {
      request.reply(status_codes::BadRequest, "invalid_email");
      return;
    }
    if (db.userExists(email)) {
      request.reply(status_codes::BadRequest, "user_exists");
      return;
    }
    if (!regex_match(password, regex("^(?=.*[a-z])(?=.*[A-Z])(?=.*[0-9]).{8,}$"))) {
      request.reply(status_codes::BadRequest, "weak_password");
      return;
    }
    std::string passwordHash = Auth::hashPassword(password);
    db.addUser(email, passwordHash);
    json::value response;
    response["user_id"] = db.getUserId(email);
    response["password_check_status"] = Auth::checkPassword(password, passwordHash) ? "perfect" : "good";
    request.reply(status_codes::OK, response);
  });

  listener.support(methods::POST, "/authorize", [](http_request request)) {
    json::value body = request.extract_json().get();
    std::string email = body["email"];
    std::string password = body["password"];
    User user = db.getUserByEmail(email);
    if (!user.isValid()) {
      request.reply(status_codes::Unauthorized, "user_not_found");
      return;
    }

    if (!Auth::checkPassword(password, user.getPasswordHash())) {
      request.reply(status_codes::Unauthorized, "invalid_password");
      return;
    }

    std::string token = (3434);
}