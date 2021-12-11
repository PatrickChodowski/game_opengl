

### documentation
https://www.boost.org/doc/libs/1_78_0/libs/python/doc/html/tutorial/tutorial/hello.html
https://www.youtube.com/watch?v=bJq1n4gQFfw&ab_channel=CppCon

### download
https://www.boost.org/users/history/version_1_78_0.html to downloads folder

### create new python venv
sudo apt install python3.8-venv
python3.8 -m venv /home/patrick/Documents/projects/game_opengl/dependencies/python


### install
cd ~/Downloads/boost_1_78_0

./bootstrap.sh --prefix=/usr --with-libraries=python




#### configure the installation -> only python, specific python installation


./bootstrap.sh --prefix=/usr--with-libraries=python

sudo ./b2 install

#### project-config.jam

import python ;
if ! [ python.configured ]
{
    using python : 3.8 : /home/patrick/Documents/projects/game_opengl/dependencies/python/bin/
}

### uninstall everything
sudo rm -f libboost_*
sudo rm -R ./boost




### need to create shared object library

gcc -shared -o libfoo.so library.o


g++ -std=c++17 modules/py.cpp -fPIC -shared -o gamescripts.so -lpython3.8 -lboost_python38 -I /usr/include/python3.8/