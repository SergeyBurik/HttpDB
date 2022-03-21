# HttpDB
HttpDB is a flexible database for mobile and web applications. Database based on HTTP protocol and REST API concepts. 

## 🎯 Features
* Easy setup - no need to configure DB connection, it works as REST API
* CRUD - store and retrieve user data
* Filtering - resource-intensive queries are memorized to optimize execution time 

## ⚙️ Installation
* compile code and run executable file  
Application will be available on `localhost:8777`

## ⚡️ Getting started
To start using the database, just send request to server

Available endpoints
```
 POST /collection/create    -> creates collection
 POST /{collection}    -> creates new document in collection
```
filtering data
```
 GET /{collection}/?key1=value1&key2=value2    -> returns query set
 GET /{collection}/?key1$::type=value    # ability to specify filter value data type (string type is default)
```

Full `CRUD` functionality is still in process of development

## How it works
Database is created with `C++`. JSON files is used to store user data. On server start, every JSON file is loaded to computer memory. All interactions with data are done in-memory. Then all changes are saved to files. 

### Screenshot
Example of filtering data in collection by selecting  
/cars?name=Ferrari
![screenshot](https://user-images.githubusercontent.com/40773987/159202103-db894e63-4433-4992-91f8-e2b219d6ef49.png)
