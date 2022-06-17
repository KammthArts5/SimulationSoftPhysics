class Mass{
    constructor(y){
        this.mass = 1;
        this.pos = y;
        this.r = 50;
        this.r0 = 200;
        this.vit=0;
        this.f=0;
    }

    initialiseMass(){
        noStroke();
        fill('red');
        circle(400, this.pos, this.r);
    }

}