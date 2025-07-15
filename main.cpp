#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <regex>

namespace fs = std::filesystem;

// Lit le contenu d'un fichier et le retourne sous forme de chaîne de caractères.
std::string readFileContent(const fs::path& filePath)
{
	std::stringstream 	buffer;
    std::ifstream 		file(filePath);
    
	buffer << file.rdbuf();
    
	return buffer.str();
}

// Utilise une expression régulière pour extraire le nom de la classe d'un fichier .hpp.
// L'expression régulière cherche le mot "class" suivi d'un ou plusieurs caractères alphanumériques (\w+).
std::string extractClassName(const std::string& fileContent)
{
    std::smatch	matches;
    std::regex	classRegex(R"(class\s+(\w+)\s*[:{])");

    // Retourne le nom de la classe trouvé.
    if (std::regex_search(fileContent, matches, classRegex) && matches.size() > 1)
        return matches[1].str();

	// Retourne une chaîne vide si aucune classe n'est trouvée.
    return "";
}

// Extrait les signatures des méthodes d'une classe à partir du contenu du fichier.
// L'expression régulière cherche un type de retour optionnel suivi d'un nom de méthode et de parenthèses.
std::vector<std::string> extractMethods(const std::string& fileContent)
{
    std::vector<std::string>	methods;
    std::regex					methodRegex(R"((\w+\s+)?(\w+)\s*\([^;]*;)");

    auto methodsBegin = std::sregex_iterator(fileContent.begin(), fileContent.end(), methodRegex);
    auto methodsEnd = std::sregex_iterator();

    for (std::sregex_iterator i = methodsBegin; i != methodsEnd; ++i)
	{
		// Ajoute le nom de la méthode trouvé.
        std::smatch match = *i;
        methods.push_back(match[2].str());
    }

    return methods;
}

// Génère le contenu du fichier .cpp en fonction du nom de la classe et de ses méthodes.
std::string generateCppContent(const std::string& className, const std::vector<std::string>& methods)
{
    std::ostringstream	cppContent;

    cppContent << "#include \"" << className << ".hpp\"\n\n";

    // Génère le constructeur de la classe.
    cppContent << className << "::" << className << "() {\n";
    cppContent << "    // Constructeur\n";
    cppContent << "}\n\n";

    // Génère le destructeur de la classe.
    cppContent << className << "::~" << className << "() {\n";
    cppContent << "    // Destructeur\n";
    cppContent << "}\n\n";

    // Génère les définitions des méthodes de la classe.
    for (const auto& method : methods)
	{
        cppContent << "void " << className << "::" << method << "() {\n";
        cppContent << "    // Implémentation de " << method << "\n";
        cppContent << "}\n\n";
    }

    return cppContent.str();
}

// Traite un fichier .hpp individuel pour générer le fichier .cpp correspondant.
void processHppFile(const fs::path& filePath)
{
    fs::path					cppPath;
    std::string					content;
    std::string					className;
    std::string 				cppContent;
    std::vector<std::string>	methods;

    content = readFileContent(filePath);
    className = extractClassName(content);
	
    if (className.empty())
		return ;
	
	methods = extractMethods(content);
    cppContent = generateCppContent(className, methods);
//	cppPath = filePath.parent_path() / (className + ".cpp");
	cppPath = fs::current_path() / (className + ".cpp");
	

    std::ofstream cppFile(cppPath);

	cppFile << cppContent;
}

// Parcourt les fichiers .hpp dans un répertoire et ses sous-répertoires, et traite chaque fichier trouvé.
void	processFiles(const fs::path& directory)
{
    for (const auto& entry : fs::recursive_directory_iterator(directory))
	{
        if (entry.is_regular_file() && entry.path().extension() == ".hpp")
            processHppFile(entry.path());
    }
}

// Fonction principale qui gère les arguments de la ligne de commande et lance le traitement des fichiers.
int main(int argc, char* argv[]) 
{
    fs::path	directory;

	directory = (argc > 1) ? fs::path(argv[1]) : fs::current_path();

    processFiles(directory);

    return 0;
}
