import json
from mongols_route import dbhelp
from website.index import db_setting


def findall(req,res,param):
    db = dbhelp(**db_setting)
    try:
        order = 'DESC'
        start = 0
        size = 5
        if 'order' in req.form:
            order = req.form['order']
        if 'start' in req.form:
            start = int(req.form['start'])
        if 'size' in req.form:
            size = int(req.form['size'])
        count = db.execute("SELECT * FROM `websites` ORDER BY `id` {} LIMIT %s,%s;".format(order),(start,size))
        if count >0:
            result = db.fetchall()
            res.set_header('Content-Type','application/json')
            res.content = json.dumps(result,ensure_ascii=False)
            res.status = 200
        db.close()
    except Exception as e:
        db.close()
        res.content = repr(e)
        res.status = 500

def handler(req,res,param):
    findall(req,res,param)

