from flask import Flask, jsonify, request, send_file, redirect, url_for
from flask_cors import CORS
from flask_jwt_extended import JWTManager, create_access_token, jwt_required
from sqlalchemy import Column, Text, VARCHAR, DECIMAL, create_engine, func
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import Session
from io import BytesIO
from dotenv import load_dotenv
from codecs import BOM_UTF16_LE
from os import getenv
from time import time
from json import load
import subprocess
import socket
import pandas as pd

load_dotenv()
with open("ACCOUNT.json") as f:
    Accounts = load(f)
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect(('127.0.0.1', 6000))
client.settimeout(0.5)
user = getenv("user")
password = getenv("pass")
SQL_IP = getenv("IP")
app = Flask(__name__)
CORS(app)
jwt = JWTManager()
jwt.init_app(app)
app.config['JWT_SECRET_KEY'] = 'JimmyWang'
app.config['JWT_ACCESS_TOKEN_EXPIRES'] = 1 # 2hrs

# ======================== segment ======================== #

@app.route('/segment/<accent>/<spell>/<out_format>' , methods=['POST'])
def article_segment(accent='1', spell='0', out_format='0'):
    global segment_result, file_name
    file = request.files['file']
    split_name = file.filename.split('.')
    file_name = split_name[0] + "_segment." + split_name[-1]

    # accent = request.args.get('accent')
    # spell = request.args.get('spell')
    # format = request.args.get('format')
    lines = file.read()
    lines += "\nend_of_article".encode('utf-16')

    process = subprocess.Popen(
        ["C:/研究所/雲端詞庫系統/Hakka_NLP_command/x64/Debug/Hakka_NLP_command.exe", accent, spell, out_format],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE)

    # process.stdin.write(lines.decode('utf-16').encode('utf-8'))
    # process.stdin.close()
    
    try:
        result, errs = process.communicate(
            input=lines.decode('utf-16').encode('utf-8'),
            timeout=15)
    except subprocess.TimeoutExpired:
        return jsonify({'msg': '斷詞錯誤'}), 415
 
    bom = BOM_UTF16_LE
    Buffer = BytesIO()
    Buffer.write(bom)
    Buffer.write(result.decode('utf-8').encode('utf-16'))
    Buffer.seek(0)
    segment_result = Buffer
    reply = {'msg': '上傳成功', 'content': result.decode('utf-8')}
    return jsonify(reply)

@app.route('/segment', methods=['GET', 'POST'])
def assignment():
    global segment_result, file_name
    if request.method == 'GET':
        return send_file(segment_result, mimetype="text/plain", as_attachment=True, download_name=file_name)
    else:
        return redirect(url_for('article_segment'))

# ======================== update ======================== #

# SQL engine create
# engine = create_engine("sqlite:///Corpus.db", connect_args={"check_same_thread": False})
# session = Session(engine)
# Base = declarative_base()
# tablename = "Hakka"
# class Table(Base):
#     __tablename__ = tablename
#     Origin = Column(Text, primary_key=True)
#     Field = Column(Text)
#     Class = Column(Text)
#     Pronounce = Column(Text)
#     Chinese = Column(Text)
#     Source = Column(Text)

#     def __init__(
#         self, Origin=None, Field=None, Class=None, 
#         Pronounce=None, Chinese=None, Source=None):
#             self.Origin = Origin
#             self.Field = Field
#             self.Class = Class
#             self.Pronounce = Pronounce
#             self.Chinese = Chinese
#             self.Source = Source

# columns = ['Origin', 'Field', 'Class', 'Pronounce', 'Chinese', 'Source']

# @app.route('/update_corpus', methods=['POST'])
# @jwt_required()
# def update_corpus():
#     #print(request.files)
#     file = request.files['file']
#     if file.filename.endswith('.csv'):
#         try:
#             df = pd.read_csv(file, dtype=str, keep_default_na=False)
#         except:
#             reply = {'msg': '檔案讀取時發生錯誤，可能為編碼或表格格式問題'}
#             return jsonify(reply), 510

#         if list(df.columns) == columns:
#             for i, data in df.iterrows():
#                 #print(data)
#                 obj = session.get(Table, {'Origin':data['Origin']})
#                 if not obj:
#                     session.add(Table(
#                         data['Origin'],
#                         data['Field'],
#                         data['Class'],
#                         data['Pronounce'],
#                         data['Chinese'],
#                         data['Source']))
#                 else:
#                     obj = session.query(Table).filter_by(Origin=data['Origin'])
#                     obj.update({
#                         'Field': data['Field'],
#                         'Class': data['Class'],
#                         'Pronounce': data['Pronounce'],
#                         'Chinese': data['Chinese'],
#                         'Source': data['Source']
#                     })

#                 session.commit()

#             reply = {'msg': '更新成功'}
#             return jsonify(reply)
#         else:
#             reply = {'msg': '表格列名錯誤'}
#             return jsonify(reply), 510
#     else:
#         reply = {'msg': '只接受CSV檔'}
#         return jsonify(reply), 415

engine2 = create_engine(f"mysql+pymysql://{user}:{password}@{SQL_IP}:3306/Hakka_Corp")
session2 = Session(engine2)
Base2 = declarative_base()
tablename2 = "Hakka"
class Table2(Base2):
    __tablename__ = tablename2
    Origin = Column('詞目', VARCHAR(20), nullable=False)
    Freq = Column('詞頻', VARCHAR(10))
    Class = Column('詞性', VARCHAR(20))
    P1 = Column('四縣腔', VARCHAR(100))
    P2 = Column('海陸腔', VARCHAR(100))
    P3 = Column('大埔腔', VARCHAR(100))
    P4 = Column('饒平腔', VARCHAR(100))
    P5 = Column('詔安腔', VARCHAR(100))
    P6 = Column('南四縣腔', VARCHAR(100))
    Chinese = Column('中客對照', VARCHAR(100))
    Source = Column('來源', VARCHAR(100))
    time = Column('更新時間', DECIMAL(15, 0))
    id = Column("ID", DECIMAL(8, 0), primary_key=True, nullable=False)

    def __init__(
        self, kwargs:dict
    ):
        self.Origin = kwargs.get('pk')
        self.Freq = kwargs.get('freq')
        self.Class = kwargs.get('class')
        self.P1 = kwargs.get('spell1', '')
        self.P2 = kwargs.get('spell2', '')
        self.P3 = kwargs.get('spell3', '')
        self.P4 = kwargs.get('spell4', '')
        self.P5 = kwargs.get('spell5', '')
        self.P6 = kwargs.get('spell6', '')
        self.Chinese = kwargs.get('chinese', '')
        self.Source = kwargs.get('source', '')
        self.time = kwargs.get('time')
        self.id = kwargs.get('id')

@app.route('/update', methods=['POST'])
@jwt_required()
def update():
    item = request.get_json()
    words = item['words']
    update_time = time()
    id = session2.query(func.max(Table2.id)).scalar()
    # print(id)
    for word in words:
        obj = session2.query(Table2).filter_by(Origin=word['pk']).first()
        if not obj:
            id += 1
            word['time'] = update_time
            word['source'] = 'Web'
            word['id'] = id
            session2.add(Table2(word))
        # else:  # 更新原有詞庫
        #     obj = session2.query(Table2).filter_by(Origin=word.pop('pk'))
        #     obj.update({
        #         'Freq':word['freq'],
        #         'Class':word['class'],
        #         'P1':word['spell'],
        #         'time':update_time
        #     })
    session2.commit()     
    reply = {'msg': '更新成功'}
    return jsonify(reply), 200

# ======================== download ======================== #

# @app.route('/download', methods=['GET'])
# @jwt_required()
# def download():
#     file_name = "output.csv"
#     format = 'utf-8'
#     csv_file = BytesIO()

#     obj = session.query(Table).all()
#     csv_file.write('Origin,Field,Class,Pronounce,Chinese,Source\n'.encode(format))
#     for row in obj:
#         row = vars(row)
#         csv_file.write((
#             f"{row['Origin']},{row['Field']},{row['Class']},"
#             f"{row['Pronounce']},{row['Chinese']},{row['Source']}\n").encode(format))
#     csv_file.seek(0)
#     return send_file(csv_file, mimetype="text/csv", as_attachment=True, download_name=file_name)

# ======================== login ======================== #

@app.route('/login', methods=['POST'])
def login_jwt():
    item = request.get_json()
    account = item['acc']
    password = item['pass']
    if Accounts.get(account, None) == password:
        access_token = create_access_token(identity=account)
        return jsonify(access_token=access_token), 200
    else:
        reply = {'msg': 'Bad account or password'}
        return jsonify(reply), 401

# ======================== split ======================== #

@app.route('/split', methods=['POST'])
def split():
    item = request.get_json()
    sent = item['sent']
    client.send(sent.encode('utf-8'))
    result = b""
    while(1):
        try:
            Rx = client.recv(1024)
        except socket.timeout:
            return jsonify({'msg':'Timeout'}), 500
        
        result += Rx
        if len(Rx) != 1024:
            break

    result = result.decode('utf-8', 'ignore').split('\n')
    num_of_words = len(result) - 2
    #print(num_of_words)
    words_list = []
    sentence = ""
    for i in range(num_of_words):
        temp = result[i].split('_')
        if temp[8] == "標點符號":
            continue
        words_list.append({
            'key': f'{i}',
            'pk': temp[0],
            'spell1': temp[1],
            'spell2': temp[2],
            'spell3': temp[3],
            'spell4': temp[4],
            'spell5': temp[5],
            'spell6': temp[6],
            'freq': temp[7],
            'class': temp[8]
        })
        sentence += f"{temp[0]}_"

    return jsonify({'words':words_list, 'sentence':sentence}), 200
