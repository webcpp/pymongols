def handler(req,res,param):
    res.set_header('Content-Type','text/plain;charset=utf-8')
    res.content='welcome to test\n'
    res.status=200
