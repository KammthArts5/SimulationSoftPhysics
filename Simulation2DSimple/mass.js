class Mass{
    constructor(x,y){
        this.mass = 1;
        this.pos = [x+Math.random()*factModifRand-factModifRand,y+Math.random()*factModifRand-factModifRand];
        this.r = 5;
        this.r0 = 11+Math.random()*factModifRand/3-factModifRand/3;
        this.r0diag = Math.sqrt(2*(this.r0**2));
        this.v=[0, 0];
        this.f=[0,0];
        
    }

    initialiseMass(){
        noStroke();
        fill('red');
        circle(this.pos[0], this.pos[1], this.r);
    }

    addForceRaideur(mass2){
        let dist = Math.sqrt(((this.pos[0]-mass2.pos[0])**2)+((this.pos[1]-mass2.pos[1])**2));
        let alpha;
        if(dist > 0.001){
            alpha = asin((mass2.pos[1]-this.pos[1])/dist);
        }
        else{
            alpha = 0;
        }
        let fBrut = -k*(dist-(this.r0+mass2.r0));


        if(this.pos[0]< mass2.pos[0]){
            this.f[0] -= Math.cos(alpha)*fBrut;
        }
        else{
            this.f[0] += Math.cos(alpha)*fBrut;
        }
        this.f[1] -= Math.sin(alpha)*fBrut;
    }

    addForceRaideurDiag(mass2){
        let dist = Math.sqrt(((this.pos[0]-mass2.pos[0])**2)+((this.pos[1]-mass2.pos[1])**2));
        let alpha;
        if(dist > 0.001){
            alpha = asin((mass2.pos[1]-this.pos[1])/dist);
        }
        else{
            alpha = 0;
        }
        let fBrut = -k*(dist-(this.r0diag+mass2.r0diag));

        if(this.pos[0]< mass2.pos[0]){
            this.f[0] -= Math.cos(alpha)*fBrut;
        }
        else{
            this.f[0] += Math.cos(alpha)*fBrut;
        }
        this.f[1] -= Math.sin(alpha)*fBrut;
    }

    addForceDamping(mass2){
        if((this.v[0]-mass2.v[0])**2  + (this.v[1]-mass2.v[1])**2 > 0.001){
            this.f[0] -= kd*(this.v[0]-mass2.v[0]);
            this.f[1] -= kd*(this.v[1]-mass2.v[1]);
        } 
        
    }

    addForcePesanteur(){
        if (this.pos[1]<= heightCanvas - floorLevel){
            this.f[1] += this.mass*g;  
        }

    }

    checkCollision(){
        if(this.pos[1] >= heightCanvas - floorLevel){
            this.pos[1] = heightCanvas - floorLevel;
            if (Math.abs(this.v[1] + Math.abs(this.v[1])) > 0.01){
                this.v[1] *= -1;
                this.v[1] *= (1-dt*facteurDecoll);
                this.v[0] *= (1-dt*facteurFrott);
            }
            
            
        }
    }

    save(){
        this.savePos = this.pos;
        this.saveV = this.v;
        this.saveF = this.f;
    }

    save2(){
        this.save2Pos = this.savepos;
        this.save2V = this.saveV;
        this.save2F = this.saveF;

    }
}