
<h1 align="center">Webserv</h1>

<p align="center">
  <img alt="Github top language" src="https://img.shields.io/github/languages/top/42vogons/webserv?color=56BEB8">

  <img alt="Github language count" src="https://img.shields.io/github/languages/count/42vogons/webserv?color=56BEB8">

  <img alt="Repository size" src="https://img.shields.io/github/repo-size/42vogons/webserv?color=56BEB8">

  <img alt="License" src="https://img.shields.io/github/license/42vogons/webserv?color=56BEB8">

  <!-- <img alt="Github issues" src="https://img.shields.io/github/issues/42vogons/webserv?color=56BEB8" /> -->

  <!-- <img alt="Github forks" src="https://img.shields.io/github/forks/42vogons/webserv?color=56BEB8" /> -->

  <!-- <img alt="Github stars" src="https://img.shields.io/github/stars/42vogons/webserv?color=56BEB8" /> -->
</p>


 <h4 align="center"> 
		:construction:  Webserv is under construction...  :construction:
</h4> 

<hr>

<p align="center">
  <a href="#about">About</a> &#xa0; | &#xa0; 
  <a href="#technologies">Technologies</a> &#xa0; | &#xa0;
  <a href="#requirements">Requirements</a> &#xa0; | &#xa0;
  <a href="#starting">Starting</a> &#xa0; | &#xa0;
  <a href="#testing">Testing</a> &#xa0; | &#xa0;
  <a href="#license">License</a> &#xa0; | &#xa0;
  <a href="https://github.com/42vogons" target="_blank">Author</a>
</p>

<br>

## About ##

This project is about writing an HTTP server.<br>
You will be able to test it with an actual browser.<br>
HTTP is one of the most used protocols on the internet.<br>
Knowing its arcane will be useful, even if you won’t be working on a website.

## Technologies ##

The following tools were used in this project:

* [C++98](https://cplusplus.com/doc/oldtutorial/)
* [Python](https://www.python.org/)
* [Go](https://go.dev/)

## Requirements ##

Before starting, you need to have [Git](https://git-scm.com) installed.

## Starting ##

```bash
# Clone this project
git clone https://github.com/42vogons/webserv

# Access
cd webserv

# Compile the project
make

# Run the project
./webserv

# The server will initialize in the <http://localhost:8080>
```
## Testing ##

```bash
# Access the test dir
cd testers

# Execute shell test
bash test.sh

# Access the concurrent_requests test dir
cd testers/concurrent_requests

# Execute concurrent_requests test
go run concurrent_requests.go [concurrentRequests] [serverURL]
# Example
go run concurrent_requests.go 999 http://localhost:8080

# or (using the compiled file - if you don't have go installed in your machine)
./concurrent_requests [concurrentRequests] [serverURL]
# Example
./concurrent_requests 999 http://localhost:8080
```

## License ##

This project is under license from MIT. For more details, see the [LICENSE](LICENSE.md) file.

&#xa0;

<a href="#top">Back to top</a>
