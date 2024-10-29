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



#database_post_args=reqparse.RequestParser()

#database_post_args.add_argument("class",type=int, help="class is required", required=True )
#database_post_args.add_argument("grade",type=int, help="grade is required", required=True )



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
    




"""
    def post(self):

        


        args=database_post_args.parse_args()
        klasa=args['class']
        ocena=args['grade']

        print(f"GRADE RECEIVED klasa: {klasa} ocena: {ocena}")
       



        return {"data": "posted"}

"""
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
            
            
        
     
       
    
    #app.run(host='192.168.0.248',port=5055,debug=False)



