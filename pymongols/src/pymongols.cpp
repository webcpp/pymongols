#include "py_http_server.hpp"
#include "py_web_server.hpp"
#include "pybind11/pybind11.h"

PYBIND11_MODULE(pymongols, m)
{
    m.doc() = "The fastest python http server,pybind11 pymongols plugin,version-0.1.8";

    pybind11::class_<mongols::request> request(m, "request");
    request.def(pybind11::init<>())
        .def_readwrite("client", &mongols::request::client)
        .def_readwrite("uri", &mongols::request::uri)
        .def_readwrite("user_agent", &mongols::request::user_agent)
        .def_readwrite("method", &mongols::request::method)
        .def_readwrite("param", &mongols::request::param)
        .def_readwrite("headers", &mongols::request::headers)
        .def_readwrite("form", &mongols::request::form)
        .def_readwrite("cache", &mongols::request::cache)
        .def_readwrite("cookies", &mongols::request::cookies)
        .def_readwrite("session", &mongols::request::session);

    pybind11::class_<mongols::response> response(m, "response");
    response.def(pybind11::init<>())
        .def_readwrite("status", &mongols::response::status)
        .def_readwrite("content", &mongols::response::content)
        .def("set_header", &mongols::response::set_header)
        .def("set_session", &mongols::response::set_session)
        .def("set_cache", &mongols::response::set_cache);

    pybind11::class_<py_http_server> http_server(m, "http_server");
    http_server.def(pybind11::init<const std::string&, int, int, size_t, size_t, size_t, int>())
        .def("run", &py_http_server::run)
        .def("add_route", &py_http_server::add_route)
        .def("run_with_route", &py_http_server::run_with_route)
        .def("set_enable_daemon", &py_http_server::set_enable_daemon)
        .def("set_pidfile", &py_http_server::set_pidfile)
        .def("set_session_expires", &py_http_server::set_session_expires)
        .def("set_cache_expires", &py_http_server::set_cache_expires)
        .def("set_enable_session", &py_http_server::set_enable_session)
        .def("set_enable_cache", &py_http_server::set_enable_cache)
        .def("set_max_open_files", &py_http_server::set_max_open_files)
        .def("set_write_buffer_size", &py_http_server::set_write_buffer_size)
        .def("set_max_file_size", &py_http_server::set_max_file_size)
        .def("set_cache_size", &py_http_server::set_cache_size)
        .def("set_enable_compression", &py_http_server::set_enable_compression)
        .def("set_db_path", &py_http_server::set_db_path)
        .def("set_enable_lru_cache", &py_http_server::set_enable_lru_cache)
        .def("set_lru_cache_expires", &py_http_server::set_lru_cache_expires)
        .def("set_lru_cache_size", &py_http_server::set_lru_cache_size)
        .def("set_openssl", &py_http_server::set_openssl)
        .def("set_enable_blacklist", &py_http_server::set_enable_blacklist)
        .def("set_enable_security_check", &py_http_server::set_enable_security_check)
        .def_static("set_blacklist_size", &py_http_server::set_blacklist_size)
        .def_static("set_blacklist_timeout", &py_http_server::set_blacklist_timeout)
        .def_static("set_max_connection_limit", &py_http_server::set_max_connection_limit)
        .def_static("set_max_send_limit", &py_http_server::set_max_send_limit)
        .def_static("set_max_connection_keepalive", &py_http_server::set_max_connection_keepalive);

    pybind11::class_<py_web_server> web_server(m, "web_server");
    web_server.def(pybind11::init<const std::string&, int, int, size_t, size_t, size_t, int>())
        .def("run", &py_web_server::run)
        .def("set_enable_daemon", &py_web_server::set_enable_daemon)
        .def("set_pidfile", &py_web_server::set_pidfile)
        .def("set_root_path", &py_web_server::set_root_path)
        .def("set_mime_type_file", &py_web_server::set_mime_type_file)
        .def("set_list_directory", &py_web_server::set_list_directory)
        .def("set_enable_mmap", &py_web_server::set_enable_mmap)
        .def("set_lru_cache_expires", &py_web_server::set_lru_cache_expires)
        .def("set_enable_lru_cache", &py_web_server::set_enable_lru_cache)
        .def("set_lru_cache_size", &py_web_server::set_lru_cache_size)
        .def("set_openssl", &py_web_server::set_openssl)
        .def("set_enable_blacklist", &py_web_server::set_enable_blacklist)
        .def("set_enable_security_check", &py_web_server::set_enable_security_check)
        .def_static("set_blacklist_size", &py_web_server::set_blacklist_size)
        .def_static("set_blacklist_timeout", &py_web_server::set_blacklist_timeout)
        .def_static("set_max_connection_limit", &py_web_server::set_max_connection_limit)
        .def_static("set_max_send_limit", &py_web_server::set_max_send_limit)
        .def_static("set_max_connection_keepalive", &py_web_server::set_max_connection_keepalive);
}
