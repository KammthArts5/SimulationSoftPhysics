function zeroNewton(f, deriv, x0, epsilon){
    let x = x0;
    let count = 0;
    while(abs(f(x))>epsilon){
        x = x - (f(x))/(deriv(x));
        // console.log(f(x));
        count ++;
        if(count>=1000){
            console.log("Fin par comptage (1000 itérations).");
            console.log("PAS DE SOLUTIONS !");
            return null;
        }
    }
    return x;

}