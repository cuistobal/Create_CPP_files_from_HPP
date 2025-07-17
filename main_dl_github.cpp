#include <iostream>
#include <cstdlib>
#include <string>

// Ce programme télécharge un dépôt GitHub passé en paramètre via git clone
// Usage: ./download_github_repo <url_du_repo>

#define USAGE_ERROR 1
#define USAGE "Usage: ./download_github_repo <url_du_repo>"

#define EMPTY_REPO "Repo URL cannot be empty."
#define EMPTY_REPO_ERROR 2

#define DOWLOAD_ERROR "Error downloading repository."
#define DOWNLOAD_ERROR_CODE 3

#define SUCCESS_MESSAGE "Repository downloaded successfully."
#define SUCCESS_CODE 0

#define CLONE_COMMAND "git clone "

static int print_status_and_return(std::string error_message, int error_code)
{
	std::cerr << error_message << std::endl;
	return error_code;
}

static int download_and_install_repo(const std::string& repo_url)
{
	int 		result;
	std::string command;

	if (repo_url.empty())
		return print_status_and_return(EMPTY_REPO, EMPTY_REPO_ERROR);

	command = CLONE_COMMAND + repo_url;
	
	result = std::system(command.c_str());
	
	if (result != 0)
		return print_status_and_return(DOWLOAD_ERROR, DOWNLOAD_ERROR_CODE);
	
	return print_status_and_return(SUCCESS_MESSAGE, SUCCESS_CODE);
}

int main(int argc, char* argv[])
{
	return argc == 2 ? download_and_install_repo(argv[1]) : print_status_and_return(USAGE, USAGE_ERROR); 
}