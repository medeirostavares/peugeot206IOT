var caixa = document.getElementById("comentarios");
var serv = document.getElementById("cont");

caixa.addEventListener("keydown", function() {
        console.log (comentarios.value.length);
    serv.innerHTML = 10 - comentarios.value.length;

})