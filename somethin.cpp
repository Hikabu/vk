#include <iostream>
#include <string>
#include <vector>

#include <cppdb/cppdb.h>
#include <jwt-cpp/jwt.h>
#include <zxcvbn/zxcvbn.h>

using namespace std;

struct User {
  int id;
  string email;
  string password;
};

cppdb::session sql;

string hash_password(const string& password) {
  return password;
}

bool is_password_weak(const string& password) {
  zxcvbn::Zxcvbn zxcvbn;
  zxcvbn::PasswordStrength strength = zxcvbn.password_strength(password);
  return strength.score < 3;
}

int register_user(const string& email, const string& password) {
  if (!regex_match(email, regex("[^@]+@[^@]+\\.[^@]+"))) {
    throw invalid_argument("Invalid email");
  }

  cppdb::result res = sql << "SELECT id FROM users WHERE email = ?" << email;
  if (res.next()) {
    throw invalid_argument("User already exists");
  }

  if (is_password_weak(password)) {
    throw invalid_argument("Password is too weak");
  }

  string hashed_password = hash_password(password);

  sql << "INSERT INTO users (email, password) VALUES (?, ?)" << email << hashed_password;

  res = sql << "SELECT LAST_INSERT_ID()";
  int user_id = res.as<int>();

  return user_id;
}

string authorize_user(const string& email, const string& password) {
  cppdb::result res = sql << "SELECT id, password FROM users WHERE email = ?" << email;
  if (!res.next()) {
    throw invalid_argument("User not found");
  }

  string hashed_password = res.as<string>("password");
  if (hashed_password != hash_password(password)) {
    throw invalid_argument("Incorrect password");
  }

  jwt::JWT jwt;
  jwt.set_issuer("auth-feed");
  jwt.set_subject(to_string(res.as<int>("id")));
  jwt.set_audience("feed");
  jwt.set_expiry(std::chrono::system_clock::now() + std::chrono::hours(1));

  return jwt.to_string();
}

bool is_token_valid(const string& token) {
  try {
    jwt::JWT jwt;
    jwt.decode(token);
    return true;
  } catch (const jwt::exception& e) {
    return false;
  }
}

void get_feed(const string& token) {
  if (!is_token_valid(token)) {
    throw invalid_argument("Invalid token");
  }

  cout << "Feed data..." << endl;
}

int main() {
  sql.open("mysql://localhost/users");

  while (true) {
    string command;
    cout << "Введите команду (register, authorize, feed, exit): ";
    cin >> command;}

    if (command == "register") {
      string email, password;
      cout << "Введите email: ";
      cin >> email;
      cout << "Введите пароль: ";
      cin >> password;}

      try {
        int user_id = register_user(email, password);
        cout << "Пользователь зарегистрирован. ID"
      }
}
        