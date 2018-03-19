#!/usr/bin/env python
# coding=utf-8
import os
	
# server
from flask import Flask, render_template

#LIRC
from lirc import Lirc

# ================================================================================================ SETUP
__location__ = os.path.realpath(os.path.join(os.getcwd(), os.path.dirname(__file__)))


# ================================================================================================ Flask
app = Flask(__name__, template_folder=__location__)
app.url_map.strict_slashes = False
app.jinja_env.trim_blocks = True
app.config['JSONIFY_PRETTYPRINT_REGULAR'] = False 

# ================================================================================================ UI
@app.route('/')  
def html():
	lirc = Lirc(os.path.join(__location__, "lirc.conf" ))
	return render_template( "remote.jinja2", client_ip = "192.168.0.51", name = lirc.name, codes = lirc.codes)

# ================================================================================================ ERROR
@app.errorhandler(404)
def page_not_found(e):
	return "NOT FOUND", 404

@app.after_request
def add_header(r):
	r.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
	r.headers["Pragma"] = "no-cache"
	r.headers["Expires"] = "0"
	r.headers['Cache-Control'] = 'public, max-age=0'
	return r

# ================================================================================================
# =================================     ENTRY POINT      =========================================
# ================================================================================================
if __name__=='__main__':
	app.run(host='0.0.0.0', port=8989, debug=False, threaded=False)
