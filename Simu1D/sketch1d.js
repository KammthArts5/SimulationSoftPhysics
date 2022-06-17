let floorLevel = 100;
let heightCanvas = 600;
let lengthCanvas = 800;

let fr = 60; //frameRate
let dt = 1/fr;

//-------------------------------------------------------------------------------
const k = 5;
const kd = 1;
const g = 9.8*100 ;
//-----------------------------------------------------------------------------
let mass1;



function setup(){
    frameRate(fr);
    createCanvas(lengthCanvas, heightCanvas);
    resetCanvas();

    mass1 = new Mass(300);
    mass1.vit = 0;
    drawBonds();
    drawMasses();

}

function draw(){
    update();
}

function update(){
    updatePhys(mass1);
    updateDraw();
}
//---------------------------------------------------------
function resetCanvas(){
    background(0);
    initFloor(floorLevel,heightCanvas);
}


function initFloor(level, heighCan){
    fill(200);
    rect(-5,heighCan-level, 1000, 10000);

}

function drawBonds(){
    stroke('white');
    strokeWeight(2);
    line(400, mass1.pos, 400, 0);
}

function drawMasses(){
   mass1.initialiseMass();
}

function drawParam(){
    textSize(17);
    fill('red');
    text('Position :', 20, 100);
    text(mass1.pos, 100, 100);
    fill('green');
    text('Vitesse :', 20, 120);
    text(mass1.vit, 100, 120);
    fill('blue');
    text('Force :', 20, 140);
    text(mass1.f, 100, 140);
}

function updateDraw(){
    resetCanvas();
    drawBonds();
    drawMasses();
    drawParam();
}

// ----------- FORCE RAIDEUR -----------



function addForceRaideur(mass1){
     mass1.f -= k*(mass1.pos-mass1.r0);
}


//  ---------------- FORCE PESANTEUR ------------------


function addForcePesanteur(masse){
     masse.f += masse.mass * g;
}


//----------------FORCE DUMPING -------------------------------------

function addForceDamping(mass1,k){

    mass1.f -= k*(mass1.vit);
}




//---------------------UPDADE VARIABLES PHYS ------------------------------
function resetForce(mass1){
    mass1.f = 0;
}


function updateForce(mass1){
    resetForce(mass1);
    addForceRaideur(mass1);
    addForceDamping(mass1, kd);
    addForcePesanteur(mass1);
}

function updateVitesse(mass1){
    mass1.vit = mass1.vit + (mass1.f/mass1.mass)*dt;
    
}

function updatePosition(mass1){
    mass1.pos = mass1.pos + mass1.vit*dt;
}

function updatePhys(mass1){
    updateForce(mass1);
    updateVitesse(mass1);
    updatePosition(mass1);
}




//-------------------------------------DEBUG------------------------------------------------------

function showForces(){
    console.log(mass1.f)
}

function showVitesse(){
    console.log(mass1.vit);
}

function showPosition(){
    console.log(mass1.pos);
}


function showData(){
    showForces();
    showVitesse();
    showPosition();
}
