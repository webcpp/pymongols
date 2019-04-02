#ifndef PY_HTTP_SERVER
#define PY_HTTP_SERVER

#include "pybind11/functional.h"
#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "pybind11/stl_bind.h"
#include <mongols/http_server.hpp>

class py_http_server {
public:
    py_http_server() = delete;
    py_http_server(const std::string& host, int port, int timeout, size_t buffer_size, size_t thread_size, size_t max_body_size, int max_event_size)
        : server(0)
    {
        this->server = new mongols::http_server(host, port, timeout, buffer_size, thread_size, max_body_size, max_event_size);
    }
    virtual ~py_http_server()
    {
        if (this->server) {
            delete this->server;
        }
    }
    void run(pybind11::function req_filter, pybind11::function res_filter)
    {
        auto f = [&](const mongols::request& req) {
            return pybind11::cast<bool>(req_filter(req));
        };
        auto g = [&](const mongols::request& req, mongols::response& res) {
            res_filter(req, &res);
        };
        this->server->run(f, g);
    }

    void add_route(const std::list<std::string>& method, const std::string& pattern, pybind11::function fun)
    {
        auto handler = [fun](const mongols::request& req, mongols::response& res, const std::vector<std::string>& param) {
            fun(req, &res, param);
        };
        this->server->add_route(method, pattern, handler);
    }

    void run_with_route(pybind11::function req_filter)
    {
        auto f = [&](const mongols::request& req) {
            return pybind11::cast<bool>(req_filter(req));
        };
        this->server->run_with_route(f);
    }
    void set_session_expires(long long expires)
    {
        this->server->set_session_expires(expires);
    }
    void set_cache_expires(long long expires)
    {
        this->server->set_cache_expires(expires);
    }
    void set_enable_session(bool b)
    {
        this->server->set_enable_session(b);
    }
    void set_enable_cache(bool b)
    {
        this->server->set_enable_cache(b);
    }
    void set_max_open_files(int len)
    {
        this->server->set_max_open_files(len);
    }
    void set_write_buffer_size(size_t size)
    {
        this->server->set_write_buffer_size(size);
    }
    void set_max_file_size(size_t size)
    {
        this->server->set_max_file_size(size);
    }
    void set_cache_size(size_t size)
    {
        this->server->set_cache_size(size);
    }
    void set_enable_compression(bool b)
    {
        this->server->set_enable_compression(b);
    }
    void set_db_path(const std::string& path)
    {
        this->server->set_db_path(path);
    }
    void set_enable_lru_cache(bool b)
    {
        this->server->set_enable_lru_cache(b);
    }
    void set_lru_cache_expires(long long expires)
    {
        this->server->set_lru_cache_expires(expires);
    }
    void set_lru_cache_size(size_t size)
    {
        this->server->set_lru_cache_size(size);
    }
    bool set_openssl(const std::string& crt, const std::string& key)
    {
        return this->server->set_openssl(crt, key);
    }
    void set_enable_blacklist(bool b)
    {
        this->server->set_enable_blacklist(b);
    }
    void set_enable_security_check(bool b)
    {
        this->server->set_enable_security_check(b);
    }

    static void set_blacklist_size(size_t size)
    {
        mongols::tcp_server::backlist_size = size;
    }
    static void set_blacklist_timeout(size_t expires)
    {
        mongols::tcp_server::backlist_timeout = expires;
    }
    static void set_max_connection_limit(size_t len)
    {
        mongols::tcp_server::max_connection_limit = len;
    }
    static void set_max_send_limit(size_t size)
    {
        mongols::tcp_server::max_send_limit = size;
    }
    static void set_max_connection_keepalive(size_t expires)
    {
        mongols::tcp_server::max_connection_keepalive = expires;
    }

private:
    mongols::http_server* server;
    std::vector<std::function<void(const mongols::request&, mongols::response&, const std::vector<std::string>&)>> route_handler;
};

#endif