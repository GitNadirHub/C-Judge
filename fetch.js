document.getElementById("runBtn").addEventListener("click", () => 
{

    const code = document.getElementById("codeArea").innerText // "<" si altele in exemplu sunt inlocuite cu alternative pentru ca "<" e deja ceva in html
                .replace(/&lt;/g, "<")                         //doar ca aici trebuie inlocuite aceste lucruri pentru ca nu se compileaza altfel
                .replace(/&gt;/g, ">");


    fetch("http://localhost:8080/run", {
        method: "POST",
        headers: { "Content-Type": "text/plain" },
        body: code
    })
    .then(res => res.text())
    .then(output => 
    {
        document.getElementById("output").innerText = output;
    })
    .catch(err => {
        document.getElementById("output").innerText = "Nu s-a putut conecta la server.";
    });
});