#ifndef PY_WEB_SERVER
#define PY_WEB_SERVER

#include <fstream>
#include <mongols/util.hpp>
#include <mongols/web_server.hpp>
#include <unistd.h>

class py_web_server {
public:
    py_web_server() = delete;
    py_web_server(const std::string& host, int port, int timeout, size_t buffer_size, size_t thread_size, size_t max_body_size, int max_event_size)
        : server(0)
        , is_daemon(false)
        , enable_multiple_processes(false)
        , pidfile()
    {
        this->server = new mongols::web_server(host, port, timeout, buffer_size, thread_size, max_body_size, max_event_size);
    }
    virtual ~py_web_server()
    {
        if (this->server) {
            delete this->server;
        }
    }
    void run()
    {
        if (this->is_daemon) {
            daemon(1, 0);
        }
        this->create_pidfile();
        auto f = [](const mongols::request& req) {
            if (req.method == "GET" && req.uri.find("..") == std::string::npos) {
                return true;
            }
            return false;
        };

        if (!this->enable_multiple_processes) {
            this->server->run(f);
        } else {
            std::function<void(pthread_mutex_t*, size_t*)> ff = [&](pthread_mutex_t* mtx, size_t* data) {
                this->server->run(f);
            };

            std::function<bool(int)> g = [&](int status) {
                return false;
            };

            mongols::multi_process main_process;
            main_process.run(ff, g);
        }
        if (!this->pidfile.empty()) {
            remove(this->pidfile.c_str());
        }
        if (this->server) {
            delete this->server;
            this->server = 0;
        }
    }
    void set_root_path(const std::string& path)
    {
        this->server->set_root_path(path);
    }
    void set_mime_type_file(const std::string& path)
    {
        this->server->set_mime_type_file(path);
    }
    void set_list_directory(bool b)
    {
        this->server->set_list_directory(b);
    }
    void set_enable_mmap(bool b)
    {
        this->server->set_enable_mmap(b);
    }
    void set_lru_cache_expires(long long expires)
    {
        this->server->set_lru_cache_expires(expires);
    }
    void set_enable_lru_cache(bool b)
    {
        this->server->set_enable_lru_cache(b);
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
    mongols::web_server* server;
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