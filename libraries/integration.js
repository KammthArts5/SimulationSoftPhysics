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


//Methodes pour les listes

function integRectList(fx, fprim, dt){
    let temp =[];
    for (let x=0; x<fx.length; x++){
        temp.push(integRect(fx[x],fprim[x], dt));
    }
    return temp;
}

function integTrapezeList(fx, fprimA, fprimB, dt){
    let temp =[];
    for (let x=0; x<fx.length; x++){
        temp.push(integTrapeze(fx[x], fprimA[x], fprimB[x], dt));
    }
    return temp;
}

function integSimpsonList(fx, fprimA, fprimMilieu, fprimB, dt){
    let temp =[];
    for (let x=0; x<fx.length; x++){
        temp.push(integSimpson(fx[x], fprimA[x], fprimMilieu[x], fprimB[x], dt));
    }
    return temp;
}

//Méthodes pour les vecteurs p5

function integRectVect(fx,fprim,dt){
    fx.add(p5.Vector.mult(fprim,dt));
}

function integTrapezeVect(fx,fprimA,fprimB, dt){
    let temp = createVector(p5.Vector.add(fprimA,fprimB));
    temp.mult(0.5*dt);
    fx.add(temp);
}

function integSimpsonVect(fx, fprimA, fprimMilieu, fprimB, dt){
    let temp = createVector(p5.Vector.add(fprimA,fprimB));
    temp.add(p5.Vector.mult(fprimMilieu,4));
    temp.mult(dt/6);
    fx.add(temp);
}