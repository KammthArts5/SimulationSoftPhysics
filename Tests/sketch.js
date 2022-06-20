const floorLevel = 300;
const heightCanvas = 1000;
const lengthCanvas = 1500;

const fr = 200; //frameRate
const dt = 1/fr;

//-------------------------------------------------------------------------------
const k = 200;
const kd = 5;
const g = 9.8;

const factModifRand = 0;
const facteurDecoll = 5;
const facteurFrott = 40;
const vitesseSim = 50;
//-----------------------------------------------------------------------------

let masses;

function setup(){
    createCanvas(100, 100);
    background(0);
    translate(50, 50);
    stroke('red');
    point(0,0);

    let v0 = createVector(0,0);
    let v1 = createVector(20, 20);
    stroke('white');
    strokeWeight(0.5);
    line(v0.x,v0.y,v1.x,v1.y);
    console.log(v1.mag());
    
    stroke('green');
    v1.setMag(-v1.mag());
    line(v0.x,v0.y,v1.x,v1.y);
    console.log(v1.mag());

}

function draw(){
    
}


