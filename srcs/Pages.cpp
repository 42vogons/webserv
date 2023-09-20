/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pages.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpereira <cpereira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:39:26 by anolivei          #+#    #+#             */
/*   Updated: 2023/09/19 23:56:17 by cpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Pages.hpp"


std::string createResponse(int code, std::string status, std::string fileContent, std::string type)
{
	std::stringstream response;
	response
		<< "HTTP/1.1 " << code << " " << status << std::endl
		<< "Content-Type: " << type << std::endl
		<< "Content-Length: " << fileContent.length() << std::endl
		<< std::endl
		<< fileContent;
	return response.str();
}

void createPage(std::string newPage, int code, std::string status, std::string& content)
{
	std::stringstream buffer;
	std::string fileContent;
	fileContent = newPage;
	content = createResponse(code, status, fileContent, "text/html");
	///std::cout << "content:::" << content << std::endl;
}

void autoIndex(std::string path)
{
	std::ofstream os;
	os.open((path + "/autoIndex.html").c_str());
	DIR *dir;
	struct dirent *ent;
	os
		<< "<html><head><title>Autoindex</title></head><body>" << std::endl
		<< "<h1>Autoindex</h1>" << std::endl;
	if ((dir = opendir(path.c_str())) != NULL)
	{
		os << "<ul>" << std::endl;
		while ((ent = readdir(dir)) != NULL)
		{
			if (std::string(ent->d_name) == "autoIndex.html")
				continue;
			os << "<li><a href=\"" << ent->d_name << "\">" << ent->d_name << "</a></li>" << std::endl;
		}
		os << "</ul>" << std::endl;
		closedir(dir);
	}
	else
		os << "<p>Error opening the directory.</p>" << std::endl;
	os << "</body></html>" << std::endl;
	os.close();
}

std::string replaceAll2(const std::string& str, const std::string& from, const std::string& to) {
    std::string result = str;
    std::string::size_type pos = 0;
    
    while ((pos = result.find(from, pos)) != std::string::npos) {
        result.replace(pos, from.length(), to);
        pos += to.length();
    }

    return result;
}

void generatePageFiles(std::string path, std::string& content, std::string pathDir, std::string pathFileError, std::string baseUrl)
{

	std::string pathFilePage = pathDir + "/files.html" ;
	
	std::ifstream file(pathFilePage.c_str());
	std::ifstream fileError(pathFileError.c_str());

	
	std::stringstream buffer;
	std::string fileContent;
	int code = 200;
	std::string status = "Ok";


	DIR *dir;
	struct dirent *ent;
	int col = 0;
	std::string html;
	html = "<table border = 1><tr><td colspan=5>Files</td></tr><tr>";
		
	if ((dir = opendir(path.c_str())) != NULL)
	{
		while ((ent = readdir(dir)) != NULL)
		{
			std::string fileName = ent->d_name;
			std::string filePath = baseUrl +"/" +fileName;
			
			if (fileName == (".") || fileName == (".."))
				continue;

			if (col == 4)
			{
				html += "</tr><tr>";
				col = -1;
			}
			col ++;
			
			html += "<td>";
			html += "<div class='image-container'>";
			html += "<a href='" + filePath+"' >" + fileName + "</a>";
			//html += "<img src='" + filePath + "' alt='"+ fileName+"' height='100' width='100'></a><br>";
			html += " <span class=\"delete-icon\" onclick=\"deleteImage('"+filePath+"')\"><img src=\"/images/lixeira.png\" height=\"20\" width=\"20\"></span></div></td>";
		}
		closedir(dir);
	}
	else
		html +="<p>Error opening the directory.</p>";
	html += "</tr></table>";

	if (file.good())
	{
		buffer << file.rdbuf();
		fileContent = buffer.str();
	}
	else
	{
		if (fileError.good())
		{
			buffer << fileError.rdbuf();
			fileContent = buffer.str();
		}
		else
		{
			fileContent = "Page not Found";
			code = 404;
			status = "Not Found";
		}
	}
	std::string text = "{{pages}}";

	
	//size_t pos = fileContent.find(text);
	
	//fileContent.replace(pos, text.length(), html);
	std::string fileContent2 = replaceAll2(fileContent, text ,html);
	
	content = createResponse(code, status, fileContent2, "text/html");
	///std::cout << "content:::" << content << std::endl;
	file.close();
	///
	//html += "</body></html>";
	//return html;
}
