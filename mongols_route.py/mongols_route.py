from functools import wraps
from jinja2 import Environment, FileSystemLoader, FileSystemBytecodeCache, select_autoescape, Template
import pymysql
from dbutils.pooled_db import PooledDB
import regex as re
import sys

__author__ = 'pangpang@hi-nginx.com'
__version__ = '0.1.11'
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
        self.route_list = []

    def route(self, pattern, method):
        def wrapper_a(func):
            self.route_list.append(
                {'method': method, 'callback': func, 'regex': re.compile(pattern)})

            @wraps(func)
            def wrapper_b(req, res, param):
                func(req, res, param)
            return wrapper_b
        return wrapper_a

    def run(self, req, res):
        for item in self.route_list:
            if req.method in item['method']:
                m = item['regex'].match(req.uri)
                if m:
                    item['callback'](req, res, m.groupdict())
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

db_setting = {'host':'127.0.0.1',
            'port':3306,
            'user':'root',
            'password':'123456',
            'database':'testdb',
            'charset':'utf8mb4',
            'creator':pymysql,
            'autocommit':True,
            'blocking':False,
            'cursorclass':pymysql.cursors.DictCursor,
            'mincached':0,#the default of 0 means no connections are made at startup
            'maxcached':0,#the default value of 0 or None means unlimited pool size
            'maxconnections':0,#the default value of 0 or None means any number of connections)
            }


class dbhelp:
    def __init__(self,**config):
        self.__config = db_setting
        self.__config.update(config)
        self.__pool = PooledDB(**self.__config)
        self.__connection = self.__pool.connection()
        self.__cursor = self.__connection.cursor()
    def execute(self,sql,*argv):
        return self.__cursor.execute(sql,*argv)
    def fetchone(self):
        return self.__cursor.fetchone()
    def fetchall(self):
        return self.__cursor.fetchall()
    def fetchmany(self,number_of_records):
        return self.__cursor.fetchmany(number_of_records)
    def commit(self):
        self.__connection.commit()
    def rollback(self):
        self.__connection.rollback()
    def close(self):
        self.__cursor.close()
        self.__connection.close()
        

