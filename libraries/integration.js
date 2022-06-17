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