import json
from mongols_route import dbhelp
from website.index import db_setting

def find(req,res,param):
    db = dbhelp(**db_setting)
    try:
        if 'id' in req.form:
            count = db.execute("SELECT * FROM `websites` WHERE `id`=%s;" , (int(req.form['id'])))
            if count>0:
                result = db.fetchone()
                res.set_header('Content-Type','application/json')
                res.content= json.dumps(result,ensure_ascii=False)
                res.status = 200
        db.close()
    except Exception as e:
        db.close()
        res.content=repr(e)
        res.status=500

def handler(req,res,param):
    find(req,res,param)

