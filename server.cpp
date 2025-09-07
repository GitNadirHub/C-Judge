#include "httplib.h"
#include <fstream>
#include <string>
#include <cstdlib>
#include <filesystem>
#include <chrono>

namespace fs = std::filesystem;

std::string readFile(const std::string& nume)
{
    std::ifstream f(nume);
    return std::string((std::istreambuf_iterator<char>(f)), {});
}

int main() {
    httplib::Server server;

    //serve html page
    server.Get("/", [](const httplib::Request&, httplib::Response& res) 
    {
        res.set_content(readFile("index.html"), "text/html");
    });

    server.Get("/fetch.js", [](const httplib::Request&, httplib::Response& res) 
    {
        res.set_content(readFile("fetch.js"), "application/javascript"); //trebuie ambele acum
    });

    server.Get("/style.css", [](const httplib::Request&, httplib::Response& res)
     {
        res.set_content(readFile("style.css"), "text/css"); //amantrei, actually

    });


    server.Post("/run", [](const httplib::Request& req, httplib::Response& res) {
        std::string code = req.body; // preia codul din textarea

        std::ofstream file("temp.cpp");
        file << code;
        file.close();
        int compileResult = system("g++ temp.cpp -o temp -mconsole 2> compile_errors.txt"); // unde "2>" indica al doilea stream aka stderr
                                                        //^-mconsole pt ca la un moment dat credea ca era winmain??????
        if (compileResult!=0) // daca e vreo eroare
        {
            res.set_content("Eroare de compilare!\n" + readFile("compile_errors.txt"), "text/plain");
            return;
        }

        std::string folder = "sum"; //folder pentru problema exemplu
        std::ostringstream results;

        int i = 0, //counter
            corecte = 0, gresite = 0;
        std::string result = "";

        for (const auto& test :  fs::directory_iterator(folder))
        {
            auto startTime = std::chrono::high_resolution_clock::now();

            std::string path = test.path().string(); //get path of test
            if (path.substr(path.size()-3) != ".in") continue; //daca nu e .in

            std::string outFile = path.substr(0, path.size() - 3) + ".out"; //.out corespunzator cu .in
            std::string tempOutput = "temp_output.txt";

            std::string command = "temp < " + path + " > " + tempOutput;
            system(command.c_str()); //convert to char pointer thing because... screw you, ig
  
            auto endTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> elapsedDuration = endTime - startTime;
            float elapsed = elapsedDuration.count();
            std::string expected = readFile(outFile);
            std::string received = readFile(tempOutput);
            
            result += std::to_string(i++);

            if (elapsed>0.5f)
            {
                result+="        Timp limita depasit!\n";
                ++gresite;
            }
            else if (expected == received) 
            {
                result += "        OK :)", ++corecte;
                result+=  "        Timp executie: " + std::to_string(elapsed) + " s \n";
            }
            else  result += "        Gresit!\n", ++gresite;
        }

        int percent = (corecte*100)/(corecte+gresite);
        std::string score = std::to_string(corecte) + "/" + std::to_string(corecte+gresite) + " puncte  (" + std::to_string(percent) + "%)\n";
        std::string content = result + score;

        res.set_content(content, "text/plain");


        //cleanup
        fs::remove("temp.cpp");
        fs::remove("temp.exe");
        fs::remove("temp_output.txt");
        fs::remove("compile_errors.txt");

    });

    server.listen("localhost", 8080);
}
