#include <Gamebuino-Meta.h>

// Metadonnées
int timeToAddLine = 300;
int percentShot = 2;

// menu variables
boolean pause = false;
boolean playerStart = false;
boolean playerWin = false;
boolean playerLoose = false;
int level = 1;

// player Stats
const int MAX_LIFE = 5;
const int SIZE_LIFE_BAR = 20;
int playerScore = 0;
int life = MAX_LIFE;
boolean bonusBulletQt = false;

int bonusTime = 0;

// Timer
long currentMillis = 0;

// Dimensions de l'écran
const int SCREEN_WIDTH = 80;
const int SCREEN_HEIGHT = 64;

// Dimensions du vaisseau
const int SHIP_WIDTH = 10;
const int SHIP_HEIGHT = 5;

// Vitesse du vaisseau
const int SHIP_SPEED = 3;

// Balles du vaisseau
const int BULLET_SPEED = 4; // Vitesse de chute des briques
const int MAX_BULLETS = 10;  // Nombre maximal de balles pouvant être tirées en même temps
int maxBullets = 3;
int bulletX[MAX_BULLETS];   // Coordonnée x de chaque balle
int bulletY[MAX_BULLETS];   // Coordonnée y de chaque balle
bool bulletActive[MAX_BULLETS];  // Indique si chaque balle est active (en vol)

// Nombre d'alien
const int MAX_ALIENS_IN_LINE = 8;
const int MAX_LINE = 50;
const int MAX_ALIENS = MAX_ALIENS_IN_LINE * MAX_LINE;
int startLine = 3;

struct Aliens{

  int x;
  int y;
  bool active;
  bool bonusBulletQt;
  bool bonusLife;
  Color color;
  
};

struct AliensBullets{

  int x;
  int y;
  int speedShoot;
  bool active;
  
};

// Dimensions de l'alien
const int ALIEN_WIDTH = 7;
const int ALIEN_HEIGHT = 5;

int shipX = (SCREEN_WIDTH - SHIP_WIDTH) / 2;   // Coordonnée x du vaisseau
int shipY = SCREEN_HEIGHT - SHIP_HEIGHT - 2;   // Coordonnée y du vaisseau



Aliens aliens[MAX_LINE][MAX_ALIENS_IN_LINE];
AliensBullets aliensBullets[MAX_LINE][MAX_ALIENS_IN_LINE];

void setup() {
  
  gb.begin();

  generateAliens();

}

void generateAliens(){

  // Position de l'alien
  int alienX = 2;
  int alienY = 6;

  Color colors[3] = {gb.createColor(60, 176, 60), gb.createColor(47, 144, 47), gb.createColor(20, 140, 20)};

  // Initialiser les aliens
  for (int i = 0; i < startLine; i++) {

    for(int j = 0; j < MAX_ALIENS_IN_LINE; j++){

      const int randomIndex = random(0, 3);
      Color randomColor = colors[randomIndex];
      
      aliens[i][j].x = alienX;
      aliens[i][j].y = alienY;
      aliens[i][j].active = true;
      aliens[i][j].color = randomColor;
      aliens[i][j].bonusBulletQt = false;
      aliens[i][j].bonusLife = false;

      if(random(100) < 50){

        if(random(100) < 5){
        
          aliens[i][j].bonusBulletQt = true;
          aliens[i][j].color = gb.createColor(255, 255, 0);

        }

        if(random(100) < 5){

          aliens[i][j].bonusBulletQt = false;
          aliens[i][j].bonusLife = true;
          aliens[i][j].color = gb.createColor(249, 96, 187);

        }
        
      }
      
      aliensBullets[i][j].x = alienX + 2;
      aliensBullets[i][j].y = alienY;
      aliensBullets[i][j].speedShoot = random(1, 3);
      aliensBullets[i][j].active = false;

      alienX += ALIEN_WIDTH + 3;
      
    }

    alienX = 2;
    alienY += ALIEN_HEIGHT + 3;
    
  }
  
}

void addAliensLine(){

  for (int i = 0; i < startLine; i++) {

    for(int j = 0; j < MAX_ALIENS_IN_LINE; j++){
      
      aliens[i][j].y += ALIEN_HEIGHT + 3;
      aliensBullets[i][j].y += ALIEN_HEIGHT + 3;
      
    }
    
  }

  // Position de l'alien
  int alienX = 2;
  int alienY = 6;

  Color colors[3] = {gb.createColor(60, 176, 60), gb.createColor(47, 144, 47), gb.createColor(20, 140, 20)};

  // Initialiser les aliens
  for (int i = startLine; i < startLine + 1; i++) {

    for(int j = 0; j < MAX_ALIENS_IN_LINE; j++){

      const int randomIndex = random(0, 3);
      Color randomColor = colors[randomIndex];
      
      aliens[i][j].x = alienX;
      aliens[i][j].y = alienY;
      aliens[i][j].active = true;
      aliens[i][j].color = randomColor;

      if(random(100) < 50){

        if(random(100) < 5){
        
          aliens[i][j].bonusBulletQt = true;
          aliens[i][j].color = gb.createColor(255, 255, 0);

        }

        if(random(100) < 5){

          aliens[i][j].bonusBulletQt = false;
          aliens[i][j].bonusLife = true;
          aliens[i][j].color = gb.createColor(249, 96, 187);

        }
        
      }
      
      aliensBullets[i][j].x = alienX + 2;
      aliensBullets[i][j].y = alienY;
      aliensBullets[i][j].speedShoot = random(1, 3);
      aliensBullets[i][j].active = false;

      alienX += ALIEN_WIDTH + 3;
      
    }
    
  }

  startLine++;
  
}

void updateShip() {
  // Déplacement du vaisseau vers la gauche
  if (gb.buttons.repeat(BUTTON_LEFT, 0) && shipX > 0) {
    shipX -= SHIP_SPEED;
  }

  // Déplacement du vaisseau vers la droite
  if (gb.buttons.repeat(BUTTON_RIGHT, 0) && shipX + SHIP_WIDTH < SCREEN_WIDTH) {
    shipX += SHIP_SPEED;
  }

  // Tirez une balle lorsque le joueur appuie sur le bouton A
  if (gb.buttons.repeat(BUTTON_A, 0)) {
    // Trouvez une balle inactive dans le tableau
    for (int i = 0; i < maxBullets; i++) {
      if (!bulletActive[i]) {
        // Tire une nouvelle balle
        bulletX[i] = shipX + SHIP_WIDTH / 2;
        bulletY[i] = shipY - 2;
        bulletActive[i] = true;
        gb.sound.tone(220, 100);
        break;  // Sort de la boucle pour éviter de tirer plusieurs balles à la fois
      }
    }
  }
  
}

void updateBullet() {
  
  for (int i = 0; i < maxBullets; i++) {
    if (bulletActive[i]) {
      // Déplacement de chaque balle vers le haut
      bulletY[i] -= BULLET_SPEED;

      // Vérifier si la balle a atteint le haut de l'écran
      if (bulletY[i] < 0) {
        bulletActive[i] = false;
      }

      // Vérifier si la balle a atteint un alien
      for (int j = 0; j < startLine; j++) {

        for(int k = 0; k < MAX_ALIENS_IN_LINE; k++){
    
            if(aliens[j][k].active && bulletX[i] >= aliens[j][k].x && bulletX[i] < aliens[j][k].x + ALIEN_WIDTH && bulletY[i] >= aliens[j][k].y && bulletY[i] < aliens[j][k].y + ALIEN_HEIGHT){
              if(aliens[j][k].bonusBulletQt && !bonusBulletQt){

                bonusBulletQt = true;
                aliens[j][k].bonusBulletQt = false;
                bonusTime = 0;
                maxBullets = 6;
                
              }
              if(aliens[j][k].bonusLife && life < MAX_LIFE){

                aliens[j][k].bonusLife = false;
                life++;
                
              }
              aliens[j][k].active = false;
              bulletActive[i] = false;
              playerScore++;
              gb.sound.tone(500, 100);
              gb.lights.fill(GREEN);
            }
        }
        
      }
      
    }
  }
}

void updateAlien() {

   // Initialiser les aliens
  for (int i = 0; i < startLine; i++) {

    for(int j = 0; j < MAX_ALIENS_IN_LINE; j++){

        if(aliens[i][j].active && !aliensBullets[i][j].active && random(100) < percentShot){

          aliensBullets[i][j].active = true;
        
        }
        
    }
    
  }
  
}

void updateAlienBullet() {

   // Initialiser les aliens
  for (int i = 0; i < startLine; i++) {

    for(int j = 0; j < MAX_ALIENS_IN_LINE; j++){

        if(aliensBullets[i][j].active){

          aliensBullets[i][j].y += aliensBullets[i][j].speedShoot;

          // Vérifier si la balle a atteint le bas de l'écran
          if (aliensBullets[i][j].y > SCREEN_HEIGHT) {
            aliensBullets[i][j].active = false;
            aliensBullets[i][j].y = aliens[i][j].y;
          }

          // Vérifier si la balle a atteint le vaisseau
          if(aliensBullets[i][j].active && aliensBullets[i][j].x >= shipX && aliensBullets[i][j].x < shipX + SHIP_WIDTH && aliensBullets[i][j].y >= shipY && aliensBullets[i][j].y < shipY + SHIP_HEIGHT){
            aliensBullets[i][j].active = false;
            life--;
            gb.sound.tone(100, 100);
            gb.lights.fill(RED);       
          }
        
        }
        
    }
    
  }
  
}

void drawShip() {
  // Dessiner le vaisseau à sa position actuelle
  gb.display.setColor(WHITE);
  gb.display.drawLine(shipX, shipY + SHIP_HEIGHT, shipX + SHIP_WIDTH / 2, shipY);
  gb.display.drawLine(shipX + SHIP_WIDTH / 2, shipY, shipX + SHIP_WIDTH, shipY + SHIP_HEIGHT);
}

void drawBullet() {
  for (int i = 0; i < maxBullets; i++) {
    if (bulletActive[i]) {
      // Dessine chaque balle à sa position actuelle
      gb.display.setColor(gb.createColor(255, 255, 0));
      gb.display.fillRect(bulletX[i], bulletY[i], 1, 3);
    }
  }
}

void drawAlien() {
  gb.display.setColor(WHITE);

   // Initialiser les aliens
  for (int i = 0; i < startLine; i++) {

    for(int j = 0; j < MAX_ALIENS_IN_LINE; j++){

       if(aliens[i][j].active){
        gb.display.setColor(aliens[i][j].color);
        gb.display.fillRect(aliens[i][j].x, aliens[i][j].y, ALIEN_WIDTH, ALIEN_HEIGHT);
      }      
    }
    
  }
}

void drawBulletAlien() {
  
  gb.display.setColor(WHITE);

   // Initialiser les aliens
  for (int i = 0; i < startLine; i++) {

    for(int j = 0; j < MAX_ALIENS_IN_LINE; j++){

       if(aliensBullets[i][j].active){
        gb.display.setColor(RED);
        gb.display.fillRect(aliensBullets[i][j].x, aliensBullets[i][j].y, 1, 3);
      }      
    }
    
  }
}

void updateMeta(){

  int countAlienNotDead = 0;

  for (int i = 0; i < startLine; i++) {

    for(int j = 0; j < MAX_ALIENS_IN_LINE; j++){

        if(aliens[i][j].active){
          countAlienNotDead++;
        }
    }
    
  }

  if(countAlienNotDead <= 0){

    level++;
    playerWin = true;
    
  }else if(life <= 0){

    playerLoose = true;
    
  }
  
}

void updateTime(){

  if(currentMillis >= timeToAddLine){

    gb.lights.fill(BLUE);
    currentMillis = 0;
    addAliensLine();
    
  }

  if(bonusBulletQt){

    const int SPHERE_RADIUS = 2;
    gb.display.setColor(gb.createColor(255, 255, 0));
    gb.display.fillCircle(SCREEN_WIDTH - (SPHERE_RADIUS * 2), 2, SPHERE_RADIUS);

    bonusTime++;

    if(bonusTime > 100){

      bonusBulletQt = false;
      maxBullets = 3;
      bonusTime = 0;
      
    }
    
  }
  
}

void drawLifeBar(){

  gb.display.setColor(WHITE);

  for(int i = 0; i < life; i++){

    gb.display.fillRect(i * (SIZE_LIFE_BAR / MAX_LIFE), 0, SIZE_LIFE_BAR / MAX_LIFE, 4);
    
  }

  gb.display.setCursor((MAX_LIFE + 1) * (SIZE_LIFE_BAR / MAX_LIFE), 0);
  gb.display.setColor(gb.createColor(255, 255, 255));
  gb.display.println(playerScore);  // Remplacer par le score de l'ordinateur
  
}

void restart(){

    pause = false;

    level = 1;
    timeToAddLine = 300;
    percentShot = 3;
    startLine = 3;
    playerScore = 0;

    for (int i = 0; i < MAX_BULLETS; i++) {

      bulletActive[i] = false;
      
    }
    
    currentMillis = 0;

    bonusBulletQt = false;
    bonusTime = 0;
    maxBullets = 3;

    life = MAX_LIFE;
    playerStart = true;
    playerWin = false;
    playerLoose = false;
    shipX = (SCREEN_WIDTH - SHIP_WIDTH) / 2;   // Coordonnée x du vaisseau
    shipY = SCREEN_HEIGHT - SHIP_HEIGHT - 2;   // Coordonnée y du vaisseau
    generateAliens();
    
}

void drawGame() {
  gb.display.clear();
  drawShip();
  drawBullet();
  drawAlien();
  drawBulletAlien();
  drawLifeBar();
}

void loop() {
  
  while (!gb.update());

  gb.lights.clear();
  currentMillis++;

  if(playerStart && !playerWin && !playerLoose){

    if(!pause){

      updateShip();
      updateBullet();
    
      if(random(100) < 5){
        updateAlien();
      }
      
      updateAlienBullet();
  
      updateMeta();
      
      drawGame();
  
      updateTime();

    }else{

      gb.display.clear();

      gb.display.setCursor(4, (SCREEN_HEIGHT / 2));
      gb.display.setColor(gb.createColor(255, 255, 255));
      gb.display.print("press B to restart");

      if (gb.buttons.pressed(BUTTON_B)) {

        restart();
        
      }
      
    }

    if (gb.buttons.pressed(BUTTON_MENU)) {

      pause = !pause;
      
    }
    
  }else{

    gb.display.clear();

    if(playerWin){

      gb.lights.fill(GREEN);  

      gb.display.setCursor((SCREEN_WIDTH / 2) - 18, (SCREEN_HEIGHT / 2) - 5);
      gb.display.setColor(gb.createColor(255, 0, 0));
      gb.display.println("GAME WIN!");  // Remplacer par le score de l'ordinateur

      gb.display.setCursor(2, (SCREEN_HEIGHT / 2) + 10);
      gb.display.setColor(gb.createColor(255, 255, 255));
      gb.display.print("press START to play");

      gb.display.setCursor(2, (SCREEN_HEIGHT / 2) + 18);
      gb.display.setColor(gb.createColor(255, 255, 255));
      gb.display.print("LEVEL: ");
      gb.display.print(level);
      
    }else if(playerLoose){

      gb.lights.fill(RED);  

      gb.display.setCursor((SCREEN_WIDTH / 2) - 18, (SCREEN_HEIGHT / 2) - 5);
      gb.display.setColor(gb.createColor(255, 0, 0));
      gb.display.println("GAME LOOSE");  // Remplacer par le score de l'ordinateur

      gb.display.setCursor(2, (SCREEN_HEIGHT / 2) + 10);
      gb.display.setColor(gb.createColor(255, 255, 255));
      gb.display.print("press START to play");
      
    }else{

      gb.display.setCursor(2, (SCREEN_HEIGHT / 2) - 5);
      gb.display.setColor(gb.createColor(255, 255, 255));
      gb.display.println("press START to play");  // Remplacer par le score de l'ordinateur

      gb.display.setCursor(2, (SCREEN_HEIGHT / 2) + 10);
      gb.display.setColor(gb.createColor(255, 255, 255));
      gb.display.print("LEVEL: ");
      gb.display.print(level);
      
    }

    // Rejouer
    if (gb.buttons.pressed(BUTTON_MENU)) {

      if(playerLoose){

        restart();
        
      }

      startLine = 4;
      timeToAddLine -= 5;
      percentShot += 0.5;

      switch (level){

        case 1:
          timeToAddLine = 300;
          percentShot = 3;
          startLine = 3;
          break;

        case 2:
          timeToAddLine = 200;
          percentShot = 4;
          startLine = 3;
          break;

        case 3:
          percentShot = 8;
          startLine = 4;
          break;
        
      }

      for (int i = 0; i < MAX_BULLETS; i++) {

        bulletActive[i] = false;
        
      }
      
      currentMillis = 0;

      bonusBulletQt = false;
      bonusTime = 0;
      maxBullets = 3;

      life = MAX_LIFE;
      playerStart = true;
      playerWin = false;
      playerLoose = false;
      shipX = (SCREEN_WIDTH - SHIP_WIDTH) / 2;   // Coordonnée x du vaisseau
      shipY = SCREEN_HEIGHT - SHIP_HEIGHT - 2;   // Coordonnée y du vaisseau
      generateAliens();

    }
    
  }
  
}
