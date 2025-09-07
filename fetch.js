document.getElementById("runBtn").addEventListener("click", () => 
{

    const code = document.getElementById("codeArea").innerText // "<" si altele in exemplu sunt inlocuite cu alternative pentru ca "<" e deja ceva in html
                .replace(/&lt;/g, "<")                         //doar ca aici trebuie inlocuite aceste lucruri pentru ca nu se compileaza altfel
                .replace(/&gt;/g, ">");



    .catch(err => {
        document.getElementById("output").innerText = "Nu s-a putut conecta la server.";
    });

});
