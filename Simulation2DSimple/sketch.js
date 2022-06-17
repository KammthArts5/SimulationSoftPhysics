const floorLevel = 300;
const heightCanvas = 1000;
const lengthCanvas = 1500;

const fr = 9000; //frameRate
const dt = 1/fr;

//-------------------------------------------------------------------------------
const k = 300;
const kd = 0.9;
const g = 9.8;

const factModifRand = 0;
const facteurDecoll = 5;
const facteurFrott = 40;
const vitesseSim = 300;
//-----------------------------------------------------------------------------

let masses;



function setup(){
    frameRate(fr);
    createCanvas(lengthCanvas, heightCanvas);
    resetCanvas();

    masses = createRectObject(50,0,6,25);
    drawBonds(masses);
    drawMasses(masses);

}

function draw(){
    //update();
}

function mouseClicked(){
    update();
}

function update(){
    for(let i = 0; i<vitesseSim; i++){
        updatePhys(masses);
        //updatePhysTrap(masses);
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

            
            for(let i=-1; i<2; i++){
                for(let j=-1; j<2; j++){
                    masses[x][y].addForcePesanteur(); //   PESANTEUR

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

function checkAllCollision(masses){
    for (let x=0; x<masses.length; x++){
        for (let y=0; y<masses[0].length; y++){
            masses[x][y].checkCollision();
        }   
    }
}




//---------------------UPDADE VARIABLES PHYS ------------------------------
function resetForce(masses){
    for (let i=0; i<masses.length; i++){
        for (let j=0; j<masses[0].length; j++){
            masses[i][j].f = [0,0];
        }
    }
}

function saveState(masses, doubleSave){
    for (let i=0; i<masses.length; i++){
        for (let j=0; j<masses[0].length; j++){
            if(doubleSave){
                masses[i][j].save2();
            }
            masses[i][j].save();
        }
    }
}


function updateVitesse(masses){
    for (let i=0; i<masses.length; i++){
        for (let j=0; j<masses[0].length; j++){
            masses[i][j].v = [integRect(masses[i][j].v[0], (masses[i][j].f[0]/masses[i][j].mass), dt), integRect(masses[i][j].v[1], (masses[i][j].f[1]/masses[i][j].mass), dt)];
        }
    }
}

function updatePosition(masses){
    for (let i=0; i<masses.length; i++){
        for (let j=0; j<masses[0].length; j++){
            masses[i][j].pos = [integRect(masses[i][j].pos[0], masses[i][j].v[0], dt), integRect(masses[i][j].pos[1], masses[i][j].v[1], dt)];
        }
    }
}

function updateVitesseTrap(masses){
    for (let i=0; i<masses.length; i++){
        for (let j=0; j<masses[0].length; j++){
            masses[i][j].v = [integTrapeze(masses[i][j].v[0], masses[i][j].f[0]/masses[i][j].mass, masses[i][j].saveF[0]/masses[i][j].mass, dt), integTrapeze(masses[i][j].v[1], masses[i][j].f[1]/masses[i][j].mass, masses[i][j].saveF[1]/masses[i][j].mass, dt)];
        }
    }
}

function updatePositionTrap(masses){
    for (let i=0; i<masses.length; i++){
        for (let j=0; j<masses[0].length; j++){
            masses[i][j].pos = [integTrapeze(masses[i][j].pos[0], masses[i][j].v[0], masses[i][j].saveV[0], dt), integTrapeze(masses[i][j].pos[1], masses[i][j].v[1], masses[i][j].saveV[1], dt)];
        }
    }
}

function updatePhys(masses){
    // resetForce(masses);
    // updateForces(masses);
    // updateVitesse(masses);
    // checkAllCollision(masses);
    // updatePosition(masses);

    for (let x=0; x<masses.length; x++){
        for (let y=0; y<masses[0].length; y++){
            masses[x][y].f = [0,0]; //reset force
            
            masses[x][y].addForcePesanteur();//   PESANTEUR
            for(let i=-1; i<2; i++){
                for(let j=-1; j<2; j++){
                    if(x+i<masses.length && x+i>=0 && y+j<masses[0].length && y+j>=0 && (i!=0 || j!=0)){
                        //masses[x][y].addForceDamping(masses[x+i][y+j]); //    DAMPING 
                        
                        if((i+j)%2 == 0){
                            masses[x][y].addForceRaideurDiag(masses[x+i][y+j]); //RAIDEUR DIAG
                        }
                        else{
                            masses[x][y].addForceRaideur(masses[x+i][y+j]); //RAIDEUR
                        }
                    }

                }
            }

            masses[x][y].v = [integRect(masses[x][y].v[0], (masses[x][y].f[0]/masses[x][y].mass), dt), integRect(masses[x][y].v[1], (masses[x][y].f[1]/masses[x][y].mass), dt)]; // update vitesse
            masses[x][y].checkCollision();
            masses[x][y].pos = [integRect(masses[x][y].pos[0], masses[x][y].v[0], dt), integRect(masses[x][y].pos[1], masses[x][y].v[1], dt)];
        }
    }
    
}

function updatePhysTrap(masses){
    resetForce(masses);
    saveState(masses, false);
    updateForces(masses);
    updateVitesseTrap(masses);
    checkAllCollision(masses);
    updatePositionTrap(masses);
    
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

