from functools import wraps
from jinja2 import Environment, FileSystemLoader, FileSystemBytecodeCache, select_autoescape, Template
import re
import sys

__author__ = 'pangpang@hi-nginx.com'
__version__ = '0.1.7'
__license__ = 'Mozilla Public License Version 2.0'

if sys.version_info.major < 3:
    reload(sys)
    sys.setdefaultencoding('utf-8')


def singleton(class_):
    instances = {}

    @wraps(class_)
    def getinstance(*args, **kwargs):
        if class_ not in instances:
            instances[class_] = class_(*args, **kwargs)
        return instances[class_]
    return getinstance


@singleton
class application:
    def __init__(self):
        self.uri_map = {}
        self.uri_regex_map = {}

    def route(self, pattern, method):
        def wrapper_a(func):
            if pattern not in self.uri_map:
                self.uri_map[pattern] = {'method': method, 'callback': func}
            if pattern not in self.uri_regex_map:
                self.uri_regex_map[pattern] = re.compile(pattern)

            @wraps(func)
            def wrapper_b(req, res, param):
                func(req, res, param)
            return wrapper_b
        return wrapper_a

    def run(self, req, res):
        items = self.uri_map.items()
        method = req.method
        reg_map = self.uri_regex_map
        for k, v in items:
            if method in v['method']:
                m = reg_map[k].match(req.uri)
                if m:
                    v['callback'](req, res, m.groupdict())
                    break


@singleton
class template:
    def __init__(self, templates_dir):
        self.templates_dir = templates_dir
        self.jinja2_env = Environment(
            loader=FileSystemLoader(self.templates_dir),
            bytecode_cache=FileSystemBytecodeCache(self.templates_dir),
            auto_reload=False,
            optimized=True,
            autoescape=select_autoescape(['htm', 'html', 'xml', 'json']))

    def file_render(self, template_file, variable):
        engine = self.jinja2_env.get_template(template_file)
        result = engine.render(variable)
        return str(result)

    def string_render(self, template_string, variable):
        engine = Template(template_string)
        result = engine.render(variable)
        return str(result)
