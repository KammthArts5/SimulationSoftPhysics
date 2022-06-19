const floorLevel = 300;
const heightCanvas = 1000;
const lengthCanvas = 1500;

const fr = 1000; //frameRate
const dt = 1/fr;

//-------------------------------------------------------------------------------
const k = 500;
const kd = 4;
const g = 9.8;

const factModifRand = 2;
const facteurDecoll = 100;
const facteurFrott = 200;
const vitesseSim = 100;
//-----------------------------------------------------------------------------

let masses;

let booltampon = false;


function setup(){
    frameRate(fr);
    createCanvas(lengthCanvas, heightCanvas);
    resetCanvas();

    masses = createRectObject(50,0,6,25);
    drawBonds(masses);
    drawMasses(masses);
    masses[0][0].v[0] = 100;

}

function draw(){
    update();
}

function mouseClicked(){
    update();
}

function update(){
    for(let i = 0; i<vitesseSim; i++){
        updateForces(masses);

        //updateInteg(masses);
        //updateIntegTrap(masses);
        updateIntegSimpson(masses)
    }
    updateDraw(masses);
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
    strokeWeight(3);
    for (let i=0; i<masses.length; i++){
        for (let j=0; j<masses[0].length; j++){
            if(i+1<masses.length){
                line(masses[i][j].pos[0], masses[i][j].pos[1], masses[i+1][j].pos[0], masses[i+1][j].pos[1]);
                if(j-1>=0){line(masses[i][j].pos[0], masses[i][j].pos[1], masses[i+1][j-1].pos[0], masses[i+1][j-1].pos[1]);}
                if(j+1<masses[0].length){line(masses[i][j].pos[0], masses[i][j].pos[1], masses[i+1][j+1].pos[0], masses[i+1][j+1].pos[1]);}
            }

            if(j+1<masses[0].length){line(masses[i][j].pos[0], masses[i][j].pos[1], masses[i][j+1].pos[0], masses[i][j+1].pos[1]);}
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
    //drawMasses(masses);
}

// ----------- CALCULS PHYSIQUE -----------

function updateForces(masses){
    for (let x=0; x<masses.length; x++){
        for (let y=0; y<masses[0].length; y++){
            masses[x][y].f = [0,0]; //reset force
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

function updateInteg(masses){
    for (let x=0; x<masses.length; x++){
        for (let y=0; y<masses[0].length; y++){
            masses[x][y].updateAccel(); //update acceleration (F/m)
            masses[x][y].v = integRectVect(masses[x][y].v, masses[x][y].a, dt); // Update vitesse
            masses[x][y].checkCollision();
            masses[x][y].pos = integRectVect(masses[x][y].pos, masses[x][y].v, dt); //update position
        }
    }  
}

function updateIntegTrap(masses){

    for (let x=0; x<masses.length; x++){
        for (let y=0; y<masses[0].length; y++){
            masses[x][y].save();
            masses[x][y].updateAccel(); //update acceleration (F/m)
            masses[x][y].v = integTrapezeVect(masses[x][y].v, masses[x][y].a, masses[x][y].saveA, dt); // Update vitesse
            masses[x][y].checkCollision();
            masses[x][y].pos = integTrapezeVect(masses[x][y].pos, masses[x][y].v, masses[x][y].saveV, dt); //update position
        }
    }
    
}

function updateIntegSimpson(masses){
    for (let x=0; x<masses.length; x++){
        for (let y=0; y<masses[0].length; y++){
            masses[x][y].save2();
            masses[x][y].save();
            masses[x][y].updateAccel(); //update acceleration (F/m)
            masses[x][y].v = integSimpsonVect(masses[x][y].v, masses[x][y].save2A, masses[x][y].saveA, masses[x][y].a, dt); // Update vitesse
            masses[x][y].checkCollision();
            masses[x][y].pos = integSimpsonVect(masses[x][y].pos, masses[x][y].save2V, masses[x][y].saveV, masses[x][y].v, dt); //update position
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

