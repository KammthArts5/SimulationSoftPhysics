class Mass{
    constructor(x,y){
        this.mass = 0.1;
        this.pos = createVector(x+random(-factModifRand, factModifRand), y+random(-factModifRand,factModifRand));
        this.r = 5;
        this.r0 = 10+Math.random()*factModifRand/3-factModifRand/3;
        this.r0diag = createVector(this.r0,this.r0).mag();
        this.v = createVector(0,0);
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
        circle(this.pos.x, this.pos.y, this.r);
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
        if(this.pos.y >= heightCanvas - floorLevel){
            this.pos.y = heightCanvas - floorLevel;
            if (abs(this.v.y + Math.abs(this.v.y)) > 0.1){
                this.v.y *= -1;
                this.v.y *= (1-dt*facteurDecoll);
                this.v.x *= (1-dt*facteurFrott);
            }
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
        let dist = this.pos.dist(mass2.pos);
        if(dist<(this.r+mass2.r)){
            //retrouver valeur de v1 et v2
            //voir comment elles évoluent et changent de sens
            //utiliser la continuité du choc
        }
    }
}