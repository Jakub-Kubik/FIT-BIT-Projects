# BUT FIT - IIS PROJECT - Costumes rental
## About
This is school project to course Information Systems. 

Information system for costumes rental developed in Django with MySQL database.

## Setup environment
Project is developing in Python in version 3.6.6

Create virtual environment:

In root folder of your project execute this commands.
- in Windows

```
python -m venv env
env/Scripts/activate
pip install -r requirements.txt
```
- in Linux/Mac
```
python -m venv env
. env/bin/activate
pip install -r requirements.txt
```

## Run app on localhost

For setup MySQL database properly on localhost create database on your localhost MySQL server and change credentials in `configs/.my.cnf` to connect to your MySQL server.

```
python manage.py makemigrations
python manage.py migrate
python manage.py runserver
```
Type `localhost:8000` in browser to display app.

## Create admin of Information system
To create your first user put this command in terminal:
```
python manage.py createsuperuser
```
You will be ask for login and password. With these credentials you will can log in to Information system as admin.

## Authors
Jan Jakub Kubik
Peter Kapicak
