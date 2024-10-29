#import easyocr

import easyocr
from flask import Flask
from flask_restful import Api, Resource, reqparse

def read_text(path):
    
    result = reader.readtext(path)

    # Print the extracted text
    for detection in result:
        print(detection[1])


print("ODPALONE")
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



    ip_address=input("ip: ")
    user_port=str(input("\nport(above 5000): "))

    while ip_address:
        try:  
            while True:
                serve(app,host=ip_address,port=user_port)
        except:
            print("error lol")
            
            continue
            