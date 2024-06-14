from flask import Flask,redirect,url_for,render_template,request,send_file
from flask_wtf import FlaskForm
from wtforms import FileField,SubmitField
import os
import subprocess
from werkzeug.utils import secure_filename
from wtforms.validators import InputRequired

global fname

app = Flask(__name__)
app.config['SECRET_KEY'] = "veysecretkey"
app.config["UPLOAD_FOLDER"] = "static/files"

class uploadedfile(FlaskForm):
    file = FileField("File" , validators=[InputRequired()])
    submit  = SubmitField("Submit")

@app.route('/')
def hello():
    return render_template('index.html')

@app.route('/compress',methods=["POST","GET"])
def compress():
    form = uploadedfile()
    if form.validate_on_submit():
        upfile = form.file.data
        upfile.save(os.path.join(os.path.abspath(os.path.dirname(__file__)),app.config["UPLOAD_FOLDER"],secure_filename(upfile.filename)))
        upfilename = upfile.filename
        global fname 
        fname = upfile.filename.split(".")[0] + "-compressed.bin" 
        print(fname+" 1here")
        subprocess.call("huffman-compress static/files/{}".format(upfilename),shell=True)
        return render_template('compress.html',form = form,codes = 1)
    else:
        return render_template('compress.html',form = form,codes = 0)


@app.route('/decompress',methods=["POST","GET"])
def decompress():
    form = uploadedfile()
    if form.validate_on_submit():
        upfile = form.file.data
        upfile.save(os.path.join(os.path.abspath(os.path.dirname(__file__)),app.config["UPLOAD_FOLDER"],secure_filename(upfile.filename)))
        upfilename = upfile.filename
        global fname 
        fname = upfile.filename.split("-")[0] + "-decompressed.txt" 
        subprocess.call("huffman-decompress static/files/{}".format(upfilename),shell=True)
        return render_template('decompress.html',form = form,codes = 1)
    else:
        return render_template('decompress.html',form = form,codes = 0)

@app.route("/download")
def download():
    global fname
    downfilename = "static/files/"+fname
    return send_file(downfilename,as_attachment=True)    

@app.route("/submit",methods=["POST","GET"])
def submit():
    if request.method == "POST":
        if request.form['but']=='compress':
            return redirect(url_for("compress"))
        else:
            return redirect(url_for("decompress"))
        

if __name__ == '__main__':
    app.run(debug=True)