#pragma once

// Optional user authentication for sd-server (enabled with --auth-file <users.json>).
//
// - Accounts with roles "admin" / "user"; passwords stored as salted PBKDF2-HMAC-SHA256 hashes.
// - The first administrator is created through /sdcpp/v1/auth/setup, from the local machine only.
// - Sessions are HMAC-signed tokens (HttpOnly SameSite=Strict cookie or "Authorization: Bearer"),
//   so they survive a server restart; changing a password invalidates that user's other sessions.
// - Every route except the WebUI page and the login/setup/status endpoints requires a session.

#include <cstdint>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "httplib.h"

struct AuthUser {
    std::string name;
    std::string role;  // "admin" or "user"
    std::string salt;  // hex
    std::string hash;  // hex, PBKDF2-HMAC-SHA256
    int iterations        = 0;
    int64_t token_version = 1;
    int64_t created       = 0;
};

struct AuthIdentity {
    std::string name;  // empty when not signed in
    std::string role;
    bool remember = false;
    bool admin() const { return role == "admin"; }
};

class AuthManager {
public:
    // Loads (or creates) the users file. Returns false and sets error on failure.
    bool open(const std::string& path, std::string& error);
    bool enabled() const { return enabled_; }

    // Pre-routing: rejects cross-origin writes and requests without a valid session.
    httplib::Server::HandlerResponse pre_route(const httplib::Request& req, httplib::Response& res);
    void register_endpoints(httplib::Server& svr);

    AuthIdentity identify(const httplib::Request& req);
    // Whether the requester may see/cancel a job created by `owner`.
    bool may_access_job(const httplib::Request& req, const std::string& owner);
    bool same_origin(const httplib::Request& req, const std::string& origin) const;

private:
    bool save_locked(std::string& error);
    AuthUser* find_locked(const std::string& name);
    size_t admin_count_locked() const;
    std::string issue_token_locked(const AuthUser& user, bool remember) const;
    void set_session_cookie(httplib::Response& res, const std::string& token, bool remember) const;

    bool enabled_ = false;
    std::string path_;
    std::vector<uint8_t> key_;  // HMAC key for session tokens, stored hex-encoded in the users file
    std::vector<AuthUser> users_;
    std::mutex mutex_;

    struct Failures {
        int count            = 0;
        int64_t locked_until = 0;
    };
    std::unordered_map<std::string, Failures> failures_;
};

// PBKDF2-HMAC-SHA256 with a 32-byte output (exposed for the self-test).
std::vector<uint8_t> auth_pbkdf2_sha256(const std::string& password, const std::vector<uint8_t>& salt, int iterations);
std::vector<uint8_t> auth_sha256(const std::string& data);
