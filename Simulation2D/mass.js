class Mass{
    constructor(x,y){
        this.mass = 0.3;
        this.pos = createVector(x+random(-factModifRand, factModifRand), y+random(-factModifRand,factModifRand));
        this.r = 5;
        this.r0 = 10+Math.random()*factModifRand/3-factModifRand/3;
        this.r0diag = createVector(this.r0,this.r0).mag();
        this.v = createVector(20,0);
        this.f=createVector(0,0);
        this.a=createVector(0,0);

        this.savePos =createVector(0,0);
        this.saveV = createVector(0,0);
        this.saveA = createVector(0,0);
        this.save2Pos =createVector(0,0);
        this.save2V = createVector(0,0);
        this.save2A = createVector(0,0);
        
    }

    initialiseMass(){
        noStroke();
        fill('red');
        circle(this.pos.x, this.pos.y, this.r*2);
    }

    updateAccel(){
        this.a = p5.Vector.div(this.f,this.mass);
    }

    addForceRaideur(mass2){
        let posRel = p5.Vector.sub(mass2.pos,this.pos);
        let temp = posRel.mag()-(this.r0+mass2.r0);
        let fact = abs(k*(temp));
        posRel.setMag(fact);

        if(temp<0){
            posRel.rotate(PI);
        }

        this.f.add(posRel);
    }

    addForceRaideurDiag(mass2){
        let posRel = p5.Vector.sub(mass2.pos,this.pos);
        let temp = posRel.mag()-(this.r0diag+mass2.r0diag);
        let fact = abs(k*(temp));
        posRel.setMag(fact);

        if(temp<0){
            posRel.rotate(PI);
        }

        this.f.add(posRel);
    }

    addForceDamping(mass2){
        let diffVit = p5.Vector.sub(mass2.v, this.v);

        diffVit.mult(kd);
        this.f.add(diffVit);
    }

    addForcePesanteur(){
        let p = createVector(0,this.mass*g);
        this.f.add(p);
    }

    checkCollisionSol(){
        if(this.pos.y+this.r >= heightCanvas - floorLevel){
            this.v.y *= -1;
            this.v.y *= (1-facteurDecoll/100);
            this.v.x *= (1-facteurFrott/100);
            this.pos.y -= 2*(this.pos.y-(heightCanvas - floorLevel-this.r));
            //à modifier avec les formules de continuité (cf youtube et le report pdf)
            
        }
    }

    save(){
        this.savePos = this.pos;
        this.saveV = this.v;
        this.saveA = this.a;
    }

    save2(){
        this.save2Pos = this.savepos;
        this.save2V = this.saveV;
        this.save2A = this.saveA;

    }

    checkCollision(mass2){
        if(this.pos.dist(mass2.pos)<(this.r+mass2.r)){
            let d = dist
            let eps1 = 0.00001;
            if(this.pos.x != 0){
                eps1 = this.pos.x;
            }
            let eps2 = 0.00001;
            if(mass2.pos.x != 0){
                eps2 = mass2.pos.x;
            }

            let p1 = this.v.y/eps1;
            let p2 = mass2.v.y/eps2;


            /*

            AJOUTER LA SUITE

            */




            let diffPos1 = p5.Vector.sub(this.pos, mass2.pos);
            let diffVit1 = p5.Vector.sub(this.pos, mass2.pos);
            let temp = diffPos1;
            let factTemp = diffVit1.dot(diffPos1) / diffPos1.magSq();
            factTemp *= -2*mass2.mass/(this.mass + mass2.mass);
            temp.mult(factTemp);

            this.v.add(temp);
        }
    }

    updateInteg(){
            this.updateAccel(); //update acceleration (F/m)
            integRectVect(this.v, this.a, dt); // Update vitesse
            integRectVect(this.pos, this.v, dt); //update position
            this.checkCollisionSol();
    }

    updateIntegTrapeze(){
        this.save();
            this.updateAccel(); //update acceleration (F/m)
            integTrapezeVect(this.v, this.a, this.saveA, dt); // Update vitesse
            integTrapezeVect(this.pos, this.v, this.saveV, dt); //update position
            this.checkCollisionSol();
    }

    updateIntegSimpson(){
        this.save2();
        this.save();
        this.updateAccel(); //update acceleration (F/m)
        integSimpsonVect(this.v, this.save2A, this.saveA, this.a, dt); // Update vitesse
        integSimpsonVect(this.pos, this.save2V, this.saveV, this.v, dt); //update position
        this.checkCollisionSol();
    }
}