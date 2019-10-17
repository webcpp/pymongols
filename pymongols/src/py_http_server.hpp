#ifndef PY_HTTP_SERVER
#define PY_HTTP_SERVER

#include "pybind11/functional.h"
#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "pybind11/stl_bind.h"
#include <fstream>
#include <mongols/http_server.hpp>
#include <mongols/util.hpp>
#include <unistd.h>

#define PYMONGOLS_LEVELDB_DIR "pymongols.leveldb"

class py_http_server {
public:
    py_http_server() = delete;
    py_http_server(const std::string& host, int port, int timeout, size_t buffer_size, size_t thread_size, size_t max_body_size, int max_event_size)
        : server(0)
        , is_daemon(false)
        , enable_multiple_processes(false)
        , pidfile()
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
        if (this->is_daemon) {
            daemon(1, 0);
        }
        this->create_pidfile();
        auto f = [&](const mongols::request& req) {
            return pybind11::cast<bool>(req_filter(req));
        };
        auto g = [&](const mongols::request& req, mongols::response& res) {
            res_filter(req, &res);
        };

        if (!this->enable_multiple_processes) {
            this->server->run(f, g);
        } else {
            std::function<void(pthread_mutex_t*, size_t*)> ff = [&](pthread_mutex_t* mtx, size_t* data) {
                std::string i;
                pthread_mutex_lock(mtx);
                if (*data > std::thread::hardware_concurrency() - 1) {
                    *data = 0;
                }
                i = std::move(std::to_string(*data));
                *data = (*data) + 1;
                pthread_mutex_unlock(mtx);
                if (!mongols::is_dir(PYMONGOLS_LEVELDB_DIR)) {
                    mkdir(PYMONGOLS_LEVELDB_DIR, S_IRUSR | S_IWUSR | S_IXUSR | S_IROTH | S_IWOTH | S_IXOTH);
                }
                this->server->set_db_path(std::string(PYMONGOLS_LEVELDB_DIR).append("/").append(i));
                this->server->run(f, g);
            };

            std::function<bool(int)> gg = [&](int status) {
                return false;
            };

            mongols::multi_process main_process;
            main_process.run(ff, gg);
        }
        if (!this->pidfile.empty()) {
            remove(this->pidfile.c_str());
        }
        if (this->server) {
            delete this->server;
            this->server = 0;
        }
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
        if (this->is_daemon) {
            daemon(1, 0);
        }
        this->create_pidfile();
        auto f = [&](const mongols::request& req) {
            return pybind11::cast<bool>(req_filter(req));
        };

        if (!this->enable_multiple_processes) {
            this->server->run_with_route(f);
        } else {
            std::function<void(pthread_mutex_t*, size_t*)> ff = [&](pthread_mutex_t* mtx, size_t* data) {
                std::string i;
                pthread_mutex_lock(mtx);
                if (*data > std::thread::hardware_concurrency() - 1) {
                    *data = 0;
                }
                i = std::move(std::to_string(*data));
                *data = (*data) + 1;
                pthread_mutex_unlock(mtx);
                if (!mongols::is_dir(PYMONGOLS_LEVELDB_DIR)) {
                    mkdir(PYMONGOLS_LEVELDB_DIR, S_IRUSR | S_IWUSR | S_IXUSR | S_IROTH | S_IWOTH | S_IXOTH);
                }
                this->server->set_db_path(std::string(PYMONGOLS_LEVELDB_DIR).append("/").append(i));
                this->server->run_with_route(f);
            };

            std::function<bool(int)> gg = [&](int status) {
                return false;
            };

            mongols::multi_process main_process;
            main_process.run(ff, gg);
        }
        if (!this->pidfile.empty()) {
            remove(this->pidfile.c_str());
        }
        if (this->server) {
            delete this->server;
            this->server = 0;
        }
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

    void set_enable_daemon(bool b)
    {
        this->is_daemon = b;
    }

    void set_enable_multiple_processes(bool b)
    {
        this->enable_multiple_processes = b;
    }

    void set_pidfile(const std::string& path)
    {
        this->pidfile = path;
    }

    void set_whitelist(const std::string& ip)
    {
        this->set_whitelist(ip);
    }
    void del_whitelist(const std::string& ip)
    {
        this->del_whitelist(ip);
    }
    void set_whitelist_file(const std::string& path)
    {
        this->set_whitelist_file(path);
    }
    void set_enable_whitelist(bool b)
    {
        this->set_enable_whitelist(b);
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
    bool is_daemon, enable_multiple_processes;
    std::string pidfile;

private:
    void create_pidfile()
    {
        if (!this->pidfile.empty()) {
            std::ofstream file;
            file.open(this->pidfile, std::ios::trunc | std::ios::out);
            file << getpid();
            file.close();
        }
    }
};

#endif