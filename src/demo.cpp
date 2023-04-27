//Yue Chen

#include <cmath>
#include <string>
#include <vector>
#include "agl/window.h"

using namespace std;
using namespace glm;
using namespace agl;

struct Particle {
  glm::vec3 pos;
  glm::vec3 vel;
  float rot;
  float size;
  bool isDone;
};

class Viewer : public Window {
public:
  Viewer() : Window() {
  }

  void setup() {
    hiScores.push_back(0);
    setWindowSize(800, 800);
    renderer.loadTexture("arrow", "../textures/arrow.png", 0);
    renderer.loadTexture("bat","../textures/bat.png",0);
    renderer.loadShader("moving-map", 
    "../shaders/moving-map.vs", 
    "../shaders/moving-map.fs");
    renderer.loadShader("billboard-animated", 
    "../shaders/billboard-animated.vs", 
    "../shaders/billboard-animated.fs");

    renderer.loadTexture("map","../textures/map.png",0);
    renderer.loadTexture("elf","../textures/character.png", 0);
    renderer.loadTexture("HP","../textures/HP.png", 0);
    renderer.loadTexture("begin","../textures/begin.png", 0);
    renderer.loadTexture("start","../textures/start.png", 0);
    renderer.loadTexture("load","../textures/loading.png", 0);
    renderer.loadTexture("pink","../textures/pink.png", 0);
    renderer.loadTexture("number","../textures/number.png", 0);
    renderer.loadTexture("ending","../textures/ending.png", 0);
    renderer.loadTexture("again","../textures/again.png", 0);
    renderer.loadTexture("catch","../textures/catch.png", 0);
    renderer.loadTexture("heart","../textures/heart.png", 0);
    renderer.loadTexture("eye","../textures/eye.png", 0);

    renderer.setDepthTest(false);
    renderer.blendMode(agl::BLEND);
  }

  void generateBat(){
    bool hasGenerated = false;
    if(rand()%1000>(990-difficulty)){
      Particle bat;
      bat.pos = vec3((rand()%81-40)/20.0,-1,0.1);
      bat.rot = 3.14;
      bat.size = 0.4;
      bat.isDone = false;
      float vel = (rand()%10+10)/10.0;
      bat.vel = vel*normalize(characterPos[pos]-bat.pos);
      for(int i =0;i<bats.size();i++){
        if(bats[i].isDone){
          bats[i]=bat;
          hasGenerated = true;
          break;
        }
      }
      if(!hasGenerated){bats.push_back(bat);}
    }
  }


  void updateConfetti(float dt)
  {
    if(direction == 1||direction ==0){direction  = (int)time/8%2 ;}
    for(int i =0;i<arrows.size();i++){
      Particle arrow = arrows[i];
      if(!arrow.isDone){
        arrow.vel += vec3(0,0.3,0)*dt;
        dt = velocity*dt;
        arrow.pos += arrow.vel * dt;
        arrow.rot = atan(arrow.vel.x/arrow.vel.y)+1.57;
        if(arrow.pos.y<-1||arrow.pos.x>1||arrow.pos.x<-1){
          arrow.isDone = true;
        }else if(arrow.vel.y>=0.0){
          arrow.isDone = true;
        }
        for(int j =0;j<bats.size();j++){
          if(bats[j].isDone){continue;}
          if(calculateD(bats[j].pos,arrow.pos)<0.1){
            arrow.isDone = true; 
            bats[j].isDone = true;
            addScore(1);
            if(rand()%100>85){
              Particle heart;
              heart.isDone = false;
              heart.rot = 3.14;
              heart.size = 0.1;
              heart.pos = bats[j].pos-vec3(0,0.2,0);
              items.push_back(heart);
            }
            break;
          }
        }
        arrows[i] = arrow;
      }
    }
    for(int i =0;i<bats.size();i++){
      Particle bat = bats[i];
      if(!bat.isDone){
        float vel = (rand()%10+5)/10.0;
        bat.vel = vel*normalize(characterPos[pos]-bat.pos);
        bat.pos += bat.vel * dt;
        if(calculateD(bat.pos,characterPos[pos])<=0.1){
          characterHP --;
          bat.isDone = true;
        }
        bats[i] = bat;
      }
    }
    for(int i =0;i<eyes.size();i++){
      Particle eye = eyes[i];
      if(!eye.isDone){
        eye.pos += eye.vel * dt;
        eye.size -= dt/20;
        if(calculateD(eye.pos,characterPos[pos])<=0.1){
          characterHP --;
          eye.isDone = true;
        }else if(eye.pos[1]>1){eye.isDone = true;addScore(1);}
        eyes[i] = eye;
      }
    }
    for(int i =0;i<catches.size();i++){
      Particle arrow = catches[i];
      if(!arrow.isDone){
        arrow.vel += vec3(0,0.3,0)*dt;
        dt = velocity*dt;
        arrow.pos += arrow.vel * dt;
        arrow.rot = atan(arrow.vel.x/arrow.vel.y)+1.57;
        if(arrow.pos.y<-1||arrow.pos.x>1||arrow.pos.x<-1){
          arrow.isDone = true;
        }else if(arrow.vel.y>=0.0){
          arrow.isDone = true;
        }
        for(int j =0;j<items.size();j++){
          if(items[j].isDone){continue;}
          if(calculateD(items[j].pos,arrow.pos)<0.1){
            arrow.isDone = true; 
            items[j].isDone = true;
            characterHP++;
            if(characterHP>4){characterHP=4;addScore(5);}
            break;
          }
        }
        catches[i] = arrow;
      }
    }
    for(int i =0;i<items.size();i++){
      if(!items[i].isDone&&(items[i].pos[0]<0.5)&&(items[i].pos[0]>-0.5)){
        items[i].pos += vec3(0,1,0)*dt;
        if(calculateD(items[i].pos,characterPos[pos])<0.2){
          items[i].isDone = true;
          characterHP++;
          if(characterHP>4){characterHP=4;addScore(5);}
        }else if(items[i].pos[1]>1){
          items[i].isDone = true;
        }
      }
    }
  }

  void addScore(int n){
    scores[0]+=n;
    for(int i = 0;i<scores.size();i++){
      if(scores[i]>=10){
        if(i<(scores.size()-1)){
          scores[i+1]++;
        }else{
          scores.push_back(1);
        }
        scores[i]-=10;
      }
    }
  }

  void showScore(float x,float y,std::vector<int> s){
    x -= ((s.size()-1)/2.0*0.08);
    renderer.beginShader("billboard-animated");
    renderer.texture("Image", "number");
    renderer.setUniform("Cols",2);
    renderer.setUniform("Rows",5);
    for(int i =0; i<scores.size();i++){
      renderer.setUniform("Frame",s[i]);
      renderer.sprite(vec3(x,y,5.0), vec4(1.0f), 0.1, 3.14);
      x += 0.06;
    }
  }

  void drawConfetti()
  {
    renderer.beginShader("sprite");
    renderer.texture("image", "arrow");
    for (int i = 0; i < arrows.size(); i++)
    {
      Particle arrow = arrows[i];
      if(!arrow.isDone){renderer.sprite(arrow.pos, vec4(1.0f), arrow.size, arrow.rot);}
    }
    renderer.texture("image", "catch");
    for (int i = 0; i < catches.size(); i++)
    {
      Particle arrow = catches[i];
      if(!arrow.isDone){renderer.sprite(arrow.pos, vec4(1.0f), arrow.size, arrow.rot);}
    }    
    renderer.texture("image", "heart");
    for (int i = 0; i < items.size(); i++)
    {
      Particle item = items[i];
      if(!item.isDone){renderer.sprite(item.pos, vec4(1.0f), item.size, item.rot);}
    }
    renderer.beginShader("billboard-animated");
    renderer.texture("Image", "bat");
    renderer.setUniform("Cols",2);
    renderer.setUniform("Rows",1);
    renderer.setUniform("Frame",(int)time/8%2);
    for (int i = 0; i < bats.size(); i++)
    {
      Particle bat = bats[i];
      if(!bat.isDone){renderer.sprite(bat.pos, vec4(1.0f), bat.size, bat.rot);}
    }
    renderer.texture("Image", "eye");
    for (int i = 0; i < eyes.size(); i++)
    {
      Particle eye = eyes[i];
      if(!eye.isDone){renderer.sprite(eye.pos, vec4(1.0f), eye.size, eye.rot);}
    }
  }

  void mouseMotion(int x, int y, int dx, int dy) {
    if((status==0)&&mouseIsDown(GLFW_MOUSE_BUTTON_LEFT)){
      float myx = x/400.0-1;
      float myy = y/400.0-1;
      if(myx<0.15&&myx>-0.15&&myy<0.1&&myy>-0.1){status = 1;}
    }
    if((status==3)&&mouseIsDown(GLFW_MOUSE_BUTTON_LEFT)){
      vec3 mouse = vec3(1-x/400.0,1-y/400.0,0);
      if(calculateD(mouse,vec3(0.0,-0.6,0))<0.1){status = 1;}
    }
    if((status==2)&&mouseIsDown(GLFW_MOUSE_BUTTON_LEFT)&&y>25){
      bool hasGenerated = false;
      Particle arrow;
      arrow.pos = characterPos[pos]-vec3(0,0.1,3);
      arrow.vel = normalize(vec3(280+(pos*120)-x,40-y,0.0));
      arrow.size = 0.2;
      arrow.isDone = false;
      arrow.rot = atan(arrow.vel.x/arrow.vel.y)+1.57;

      for(int i = 0; i<arrows.size();i++){
        if(arrows[i].isDone){
          arrows[i]=arrow;
          hasGenerated = true;
          break;
        }
      }
      if(!hasGenerated&&arrows.size()<11){arrows.push_back(arrow);}
    }else if((status==2)&&mouseIsDown(GLFW_MOUSE_BUTTON_RIGHT)&&y>25){
      bool hasGenerated = false;
      Particle arrow;
      arrow.pos = characterPos[pos]-vec3(0,0.1,3);
      arrow.vel = normalize(vec3(280+(pos*120)-x,40-y,0.0));
      arrow.size = 0.2;
      arrow.isDone = false;
      arrow.rot = atan(arrow.vel.x/arrow.vel.y)+1.57;

      for(int i = 0; i<catches.size();i++){
        if(catches[i].isDone){
          catches[i]=arrow;
          hasGenerated = true;
          break;
        }
      }
      if(!hasGenerated&&catches.size()<4){catches.push_back(arrow);}
    }
    if(x>450){direction = leftPos;}
    else if(x<350){direction = rightPos;}
    else{direction = 0;}
    
  }

  void keyUp(int key, int mods) {
    if(key==GLFW_KEY_A){
      pos --;
      if(pos<0){pos=0;}
    }
    if(key==GLFW_KEY_D){
      pos++;
      if(pos>2){pos=2;}
    }
  }
  void mouseDown(int button, int mods) {
    
  }

  void mouseUp(int button, int mods) {
  }

  void generateEye(){
    bool hasGenerated = false;
    if(rand()%1000<(10+difficulty)){
      Particle eye;
      eye.pos = characterPos[rand()%3]-vec3(0,2,0);
      eye.rot = 3.14;
      eye.size = 0.4;
      eye.isDone = false;
      float vel = (rand()%10+10)/10.0;
      eye.vel = vec3(0,1,0);
      for(int i =0;i<eyes.size();i++){
        if(eyes[i].isDone){
          eyes[i]=eye;
          hasGenerated = true;
          break;
        }
      }
      if(!hasGenerated&&eyes.size()<5){eyes.push_back(eye);}
    }
  }

  float calculateD(vec3 a, vec3 b){
    float x1 = a.x;
    float x2 = b.x;
    float y1 = a.y;
    float y2 = b.y;
    float d = sqrt(pow(x1-x2,2)+pow(y1-y2,2));
    return d;
  }

  int convertS(std::vector<int> s){
    int sc = s[0];
    for(int i = 1;i<s.size();i++){
      sc *= 10;
      sc += s[i];
    }
    return sc;
  }

  void drawScene(){
    //draw map
    renderer.beginShader("moving-map");
    //time should be [0,128];
    time = time + dt()* 30.0f;
    if(time >240.0f){
      time -= 240.0f;
    }
    renderer.setUniform("Time", time);

    renderer.texture("Image", "map");
    renderer.sprite(vec3(0,0,10), vec4(1.0f), 2.0);

    //draw the character
    renderer.beginShader("billboard-animated");
    renderer.texture("Image", "elf");
    renderer.setUniform("Cols",2);
    renderer.setUniform("Rows",2);
    
    renderer.setUniform("Frame",direction);
    renderer.sprite(characterPos[pos], vec4(1.0f), 0.5, 3.14);

    //draw the characterHP
    renderer.beginShader("billboard-animated");
    renderer.texture("Image", "HP");
    renderer.setUniform("Cols",2);
    renderer.setUniform("Rows",2);
    renderer.setUniform("Frame",characterHP);
    renderer.sprite(vec3(-0.7,0.7,5.0), vec4(1.0f), 0.5, 3.14);

    showScore(0.7,0.7,scores);
  }

  void drawBegin(){
    time = time + dt()* 30.0f;
    if(time >240.0f){
      time -= 240.0f;
      difficulty += 0.2;
    }
    renderer.beginShader("billboard-animated");
    renderer.texture("Image", "begin");
    renderer.setUniform("Cols",2);
    renderer.setUniform("Rows",2);
    renderer.setUniform("Frame",3-(int)time/6%4);
    renderer.sprite(vec3(0,0,10), vec4(1.0f), 2.0,3.14);
  }

  void drawEnd(){    
    renderer.beginShader("moving-map");
    //time should be [0,128];
    time = time + dt()* 30.0f;
    if(time >240.0f){
      time -= 240.0f;
    }
    renderer.setUniform("Time", time);
    renderer.texture("Image", "map");
    renderer.sprite(vec3(0,0,10), vec4(1.0f), 2.0);

    renderer.beginShader("sprite");
    renderer.texture("Image","ending");
    renderer.sprite(vec3(0,-0.2,0), vec4(1.0f), 2.0,3.14);

    showScore(0,-0.4,scores);
    showScore(0.6,0.65,hiScores);

    renderer.texture("Image", "again");
    drawButton(0.0,-0.6);
  }

  void drawButton(float x,float y){
    renderer.setUniform("Cols",2);
    renderer.setUniform("Rows",1);
    renderer.setUniform("Frame",(int)time/6%2);
    renderer.sprite(vec3(x,y,1), vec4(1.0f), 0.3,3.14);
  }

  void loading(float dt){
    renderer.beginShader("sprite");
    renderer.texture("Image","pink");
    load -= 2*dt;
    renderer.sprite(vec3(load,0,0), vec4(1.0f), 2.0,3.14);
    renderer.texture("Image","load");
    renderer.sprite(vec3(0,0,1.0), vec4(1.0f), 2.0,3.14);
    if(load <0){status = 2;}
  }

  
  void draw() {
    renderer.ortho(-1,1,-1,1,0,100);
    renderer.lookAt(eyePos, lookPos, up);
    if(status == 0){
      drawBegin();
      renderer.texture("Image", "start");
      drawButton(0.0,-0.06);
    }
    if(status == 1){
      scores.clear();
      scores.push_back(0);
      loading(dt());
    }
    if(status == 2){
      drawScene();
      generateBat();
      generateEye();
      updateConfetti(dt());
      drawConfetti();
      renderer.endShader();
      //game ends, intializing the data
      if(characterHP==1){
        if(convertS(scores)>convertS(hiScores)){hiScores = scores; }
        status=3;
        load = 2.0;
        for(int i = 0;i<bats.size();i++){
          bats[i].isDone = true;
        }
        for(int i = 0;i<arrows.size();i++){
          arrows[i].isDone = true;
        }
        for(int i = 0;i<items.size();i++){
          items[i].isDone = true;
        }
        for(int i = 0;i<eyes.size();i++){
          eyes[i].isDone = true;
        }
        characterHP = 4;
        difficulty = 1.0;
      }
    }
    if(status == 3){
      drawEnd();
    }
  }

protected:

  vec3 eyePos = vec3(0, 0, -10);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);

  //Chatacter's posture
  int downPos = 0;
  int leftPos = 2;
  int rightPos = 3;

  float difficulty = 1.0;
  float time = 0;
  int characterHP = 4;
  int direction = downPos; 
  float velocity = 1.0;
  int score = 0;

  std::vector<int> scores;
  std::vector<int> hiScores;

  int status = 0;
  float load = 2.0;

  std::vector<Particle> catches;
  std::vector<Particle> items;
  std::vector<Particle> arrows;
  std::vector<Particle> bats;
  std::vector<Particle> eyes;

  int pos = 1;
  vec3 characterPos[3]= {vec3(0.3,0.75,3),vec3(0.0,0.75,3),vec3(-0.3,0.75,3)};

};

int main(int argc, char** argv)
{
  Viewer viewer;
  viewer.run();
  return 0;
}
