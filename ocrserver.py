#import easyocr

import easyocr
from flask import Flask
from flask_restful import Api, Resource, reqparse
import os

def read_text(path):
    
    result = reader.readtext(path)

    # Print the extracted text
    for detection in result:
        print(detection[1])


print("Ocr resources loaded")
reader = easyocr.Reader(['en'],gpu=True)
app =Flask(__name__)
api = Api(app)


database_get_args= reqparse.RequestParser()
database_get_args.add_argument("filepath",type=str, help="path to file is required. . .", required=True,location='args')


class HelloWorld(Resource):
    def get(self):

        args=database_get_args.parse_args()
        path = args['filepath']
    

        result=reader.readtext(path,paragraph=True,x_ths=0.5,y_ths=90)
        
        
        return_table=[]
        
        
        for detection in result:
            print(detection[1])
            return_table.append(detection[1])
        
                    
       
        
        print({"GRADES SENT": return_table})
        return {"purpose": "sendingResults","return_table": return_table}
    


api.add_resource(HelloWorld,"/ocr")


if __name__=="__main__":
    from waitress import serve

    dirname = os.path.dirname(__file__)
    filename = os.path.join(dirname, 'tool_config.txt')
    
    ocrIp=str()
    ocrPort=str()
    
    
    with open(filename,"r") as file:
        for line in file:
            if "ocrIp:" in line:
                ocrIp=line[6:-1]
            if "ocrPort" in line:    
                ocrPort=line[8:-1]
    
    ocrIp=ocrIp.strip()
    ocrPort=ocrPort.strip()
    
    

    while ocrIp:
        try:  
            while True:
                print(f"Server hosted successfully on: {ocrIp}: {ocrPort}")
                serve(app,host=ocrIp,port=ocrPort)
        except:
            print("error lol")
            
            continue
            