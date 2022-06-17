//Librairie perso de méthodes d'intégrations de fonctions en JavaScript

function integRect(fx, fprim, dt){ //intégration rectangle droite, gauche, milieu en changeant ce qu'on met en fprim
    return fx + fprim*dt;
}

function integTrapeze(fx, fprimA, fprimB, dt){ //intégration méthode des trapèzes
    return fx + dt * (fprimA+fprimB)/2;
}

function integSimpson(fx,fprimA, fprimMilieu, fprimB, dt){
    return fx + (dt/6) *(fprimA + 4*fprimMilieu + fprimB);
}


//Methodes pour les vecteurs

function integRectVect(fx, fprim, dt){
    let temp =[];
    for (let x=0; x<fx.length; x++){
        temp.push(integRect(fx[x],fprim[x], dt));
    }
    return temp;
}

function integTrapezeVect(fx, fprimA, fprimB, dt){
    let temp =[];
    for (let x=0; x<fx.length; x++){
        temp.push(integTrapeze(fx[x], fprimA[x], fprimB[x], dt));
    }
    return temp;
}

function integSimpsonVect(fx, fprimA, fprimMilieu, fprimB, dt){
    let temp =[];
    for (let x=0; x<fx.length; x++){
        temp.push(integSimpson(fx[x], fprimA[x], fprimMilieu[x], fprimB[x], dt));
    }
    return temp;
}