const floorLevel = 300;
const heightCanvas = 1000;
const lengthCanvas = 1500;

const fr = 1000; //frameRate
const dt = 1/fr;

//-------------------------------------------------------------------------------
const k = 90;
const kd = 0.8;
const g = 9.8;

const factModifRand = 6;
const facteurDecoll = 0.1;
const facteurFrott = 1;
const vitesseSim = 40;
//-----------------------------------------------------------------------------

let masses;

let pauseState = false;

function setup(){
    frameRate(fr);
    createCanvas(lengthCanvas, heightCanvas);
    resetCanvas();

    masses = createRectObject(200,100,5,20);
    drawBonds(masses);
    drawMasses(masses);

}

function draw(){
    if(!pauseState){
        update();
    }
}

function mouseClicked(){
    update();
}

function keyPressed(){ //FONCTION pauseState
    if(key == 'p'){
        pause();    
    }
}

function pause(){
    pauseState = !pauseState;

    fill('green');
    textFont('Helvetica');
    textSize(120);
    textAlign(CENTER);
    text("PAUSE", width/2, height/2);
}

function update(){
    for(let i = 0; i<vitesseSim; i++){
        updateForces(masses);

        updateAllInteg(masses);
        //updateAllIntegTrap(masses);
        //updateAllIntegSimpson(masses)
    }

    updateDraw(masses);

    console.log
}
//---------------------------------------------------------
function resetCanvas(){
    background(0);
    initFloor(floorLevel,heightCanvas);
}


function initFloor(level, heighCan){
    noStroke();
    fill(200);
    rect(-5,heighCan-level, 10000000, 10000);

}

function createRectObject(x,y,n,m){
    let masses = [];
    
    for (let i=0; i<n; i++){
        masses.push([]);
        for (let j=0; j<m; j++){
            masses[i].push(new Mass(x+20*i, y+20*j));
        }
    }

    return masses;
}

function drawBonds(masses){
    stroke('white');
    strokeWeight(1);
    for (let i=0; i<masses.length; i++){
        for (let j=0; j<masses[0].length; j++){
            if(i+1<masses.length){
                line(masses[i][j].pos.x, masses[i][j].pos.y, masses[i+1][j].pos.x, masses[i+1][j].pos.y);
                if(j-1>=0){line(masses[i][j].pos.x, masses[i][j].pos.y, masses[i+1][j-1].pos.x, masses[i+1][j-1].pos.y);}
                if(j+1<masses[0].length){line(masses[i][j].pos.x, masses[i][j].pos.y, masses[i+1][j+1].pos.x, masses[i+1][j+1].pos.y);}
            }

            if(j+1<masses[0].length){line(masses[i][j].pos.x, masses[i][j].pos.y, masses[i][j+1].pos.x, masses[i][j+1].pos.y);}
        }
    }
}

function drawMasses(masses){
    for (let i=0; i<masses.length; i++){
        for (let j=0; j<masses[0].length; j++){
            masses[i][j].initialiseMass();
        }
    }
}

function updateDraw(masses){
    resetCanvas();
    drawBonds(masses);
    drawMasses(masses);
}

// ----------- CALCULS PHYSIQUE -----------

function updateForces(masses){
    for (let x=0; x<masses.length; x++){
        for (let y=0; y<masses[0].length; y++){
            masses[x][y].f.x = 0;
            masses[x][y].f.y = 0; //reset force
            masses[x][y].addForcePesanteur(); //   PESANTEUR
            for(let i=-1; i<2; i++){
                for(let j=-1; j<2; j++){
                    
                    if(x+i<masses.length && x+i>=0 && y+j<masses[0].length && y+j>=0 && (i!=0 || j!=0)){
                        masses[x][y].addForceDamping(masses[x+i][y+j]); //    DAMPING 
                        
                        if((i+j)%2 == 0){
                            masses[x][y].addForceRaideurDiag(masses[x+i][y+j]); //RAIDEUR DIAG
                        }
                        else{
                            masses[x][y].addForceRaideur(masses[x+i][y+j]); //RAIDEUR
                        }
                    }

                }
            }

        }   
    }
}




//---------------------UPDADE VARIABLES PHYS ------------------------------

function updateAllInteg(masses){
    for (let x=0; x<masses.length; x++){
        for (let y=0; y<masses[0].length; y++){
            masses[x][y].updateInteg();

            for(let i=-1; i<2; i++){
                for(let j=-1; j<2; j++){
                    if(x+i<masses.length && x+i>=0 && y+j<masses[0].length && y+j>=0 && (i!=0 || j!=0)){
                        masses[x][y].checkCollision(masses[x+i][y+j]);
                    }
                }
            }
        }
    }  
}

function updateAllIntegTrap(masses){

    for (let x=0; x<masses.length; x++){
        for (let y=0; y<masses[0].length; y++){
            masses[x][y].updateIntegTrap();
        }
    }
    
}

function updateAllIntegSimpson(masses){
    for (let x=0; x<masses.length; x++){
        for (let y=0; y<masses[0].length; y++){
           masses[x][y].updateIntegSimpson();
        }
    }
}




//-------------------------------------DEBUG------------------------------------------------------

function showForces(){
    let table = [];
    for (let i=0; i<masses.length; i++){
        table.push([]);
        for (let j=0; j<masses[0].length; j++){
            let str1 = String(masses[i][j].f[0]);
            let str2 = String(masses[i][j].f[1]);
            table[i].push(str1 + ',' + str2);
        }
    }

    console.table(table);
}

function showVitesse(){
    let table = [];
    for (let i=0; i<masses.length; i++){
        table.push([]);
        for (let j=0; j<masses[0].length; j++){
            let str1 = String(masses[i][j].v[0]);
            let str2 = String(masses[i][j].v[1]);
            table[i].push(str1 + ',' + str2);
        }
    }

    console.table(table);
}

function showPosition(){
    let table = [];
    for (let i=0; i<masses.length; i++){
        table.push([]);
        for (let j=0; j<masses[0].length; j++){
            let str1 = String(masses[i][j].pos[0]);
            let str2 = String(masses[i][j].pos[1]);
            table[i].push(str1 + ',' + str2);
        }
    }

    console.table(table);
}

