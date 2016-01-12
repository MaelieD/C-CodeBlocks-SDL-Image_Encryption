import os



if __name__ == '__main__':
   path = 'images/lena100/'
   print os.listdir(path)


   for file in os.listdir(path):
      print 'projet.exe images/lena100/'+file
      os.system('projet.exe images/lena100/'+file + ' images/lena100C/'+file)
