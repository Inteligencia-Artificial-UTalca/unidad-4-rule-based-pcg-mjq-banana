#include <iostream>
#include <vector>
#include <random>   // For random number generation
#include <chrono>   // For seeding the random number generator

// Define Map as a vector of vectors of integers.
// You can change 'int' to whatever type best represents your cells (e.g., char, bool).
using Map = std::vector<std::vector<int>>;

/**
 * @brief Prints the map (matrix) to the console.
 * @param map The map to print.
 */
void printMap(const Map& map) {
    std::cout << "--- Current Map ---" << std::endl;
    for (const auto& row : map) {
        for (int cell : row) {
            // Adapt this to represent your cells meaningfully (e.g., ' ' for empty, '#' for occupied).
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "-------------------" << std::endl;
}

/**
 * @brief Function to implement the Cellular Automata logic.
 * It should take a map and return the updated map after one iteration.
 * @param currentMap The map in its current state.
 * @param W Width of the map.
 * @param H Height of the map.
 * @param R Radius of the neighbor window (e.g., 1 for 3x3, 2 for 5x5).
 * @param U Threshold to decide if the current cell becomes 1 or 0.
 * @return The map after applying the cellular automata rules.
 */
Map cellularAutomata(const Map& currentMap, int W, int H, int R, double U) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    srand(seed);
    Map newMap = currentMap; // Initially, the new map is a copy of the current one
   
    //se genera el ruido
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            newMap[x][y] = ((rand() % 30) < 2) ? 1 : 0;
        }
    }
    Map noisyMap = newMap;
    //se recorre el mapa nuevo
    for (int y = 0; y < H ; y++){
        for (int x = 0; x < W; x ++){
            //una vez voy recorriendo el mapa debo recorrer el area al rededor del punto para revisar si tiene los valores necesarios para difuminar
            int count = 0;
            for(int radY = - R; radY <= R; radY++){
                for (int radX = - R; radX <= R; radX++){
                    int nx = x + radX;
                    int ny = y + radY;

                     if (nx < 0 || nx >= W || ny < 0 || ny >= H) {//si sale del mapa, salta ese paso
            
                        count += 1;
                    }else{
                        count += newMap[nx][ny];
                    }
                    
                }
            }
            noisyMap[x][y] = (count>= U) ? 0:1;
        }
    }
    // TODO: IMPLEMENTATION GOES HERE for the cellular automata logic.
    // Iterate over each cell and apply the transition rules.
    // Remember that updates should be based on the 'currentMap' state
    // and applied to the 'newMap' to avoid race conditions within the same iteration.

    return noisyMap;
}


/**
 * @brief Function to implement the Drunk Agent logic.
 * It should take a map and parameters controlling the agent's behavior,
 * then return the updated map after the agent performs its actions.
 *
 * @param currentMap The map in its current state.
 * @param W Width of the map.
 * @param H Height of the map.
 * @param J The number of times the agent "walks" (initiates a path).
 * @param I The number of steps the agent takes per "walk".
 * @param roomSizeX Max width of rooms the agent can generate.
 * @param roomSizeY Max height of rooms the agent can generate.
 * @param probGenerateRoom Probability (0.0 to 1.0) of generating a room at each step.
 * @param probIncreaseRoom If no room is generated, this value increases probGenerateRoom.
 * @param probChangeDirection Probability (0.0 to 1.0) of changing direction at each step.
 * @param probIncreaseChange If direction is not changed, this value increases probChangeDirection.
 * @param agentX Current X position of the agent (updated by reference).
 * @param agentY Current Y position of the agent (updated by reference).
 * @return The map after the agent's movements and actions.
 */
Map drunkAgent(const Map& currentMap, int W, int H, int J, int I, int roomSizeX, int roomSizeY,
               double probGenerateRoom, double probIncreaseRoom,
               double probChangeDirection, double probIncreaseChange,
               int& agentX, int& agentY) {
unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    std::mt19937 generator(seed);
    srand(seed);
    std::cout << "--- DRUNK SIMULATION ACTIVE ---" << std::endl;
    Map newMap = currentMap; // The new map is a copy of the current one
    //bool complete_move = false;
    std::uniform_int_distribution<int> directions(0, 3); //Posibles direcciones a tomar 
    newMap[agentY][agentX] = 2; //Posicion inicial del agente
    auto probabilidadAux = probChangeDirection;
    auto probabilidadRoomAux = probGenerateRoom;
    bool CreateRoom = false;
    int direct = 0; //Valor del switch para direcciones
    for(int move = 0; move < J; move++){ //Movimientos que dara el agente
        //debería crear una variable que me provea la dirección previa
        auto auxDir = direct;
        
        if((std::rand() % 1,0) <= probabilidadAux){//si el numero aleatorio coincide a ser menor que la probabilidad aux
            direct = directions(generator); //Direccion aleatoria    entonces cambia de dirección random
        }
        //pregunto si son iguales
        if(auxDir == direct){
            probIncreaseChange++;
            probabilidadAux = probIncreaseChange;
        }
        else{
            probabilidadAux = probChangeDirection;
        }

        std::cout << "Movimiento: " << move + 1 << " - Direccion: " << direct << std::endl;
        switch (direct)
        {
        case 0: //arriba ----
            for(int walk = 0; walk < I; walk++){ //Pasos del agente
                if(agentY <= 0){ //Si topan el muro
                    break; //Se rompe el ciclo
                }
                newMap[agentY--][agentX] = 1; //Se mueve el agente
            }
            break;
        case 1: //abajo ----//
            for(int walk = 0; walk < I; walk++){
                if(agentY >= W - 1){
                    break;
                }
                newMap[agentY++][agentX] = 1;
            }
            break;
        case 2: //izquierda ----
            for(int walk = 0; walk < I; walk++){
                if(agentX <= 0){
                    break;
                }
                newMap[agentY][agentX--] = 1;
            }
            break;
        case 3: //derecha ----
            for(int walk = 0; walk < I; walk++){
                if(agentX >= H - 1){
                    break;
                }
                newMap[agentY][agentX++] = 1;
            }
            break;
        }
        //una vez llega tiene que dibujar la room
        if((rand() % 1,0) < probabilidadRoomAux){
            CreateRoom = true;
            auto BordeX = roomSizeX + agentX;
            auto BordeY = roomSizeY+ agentY;
            //auxiliares para poder saber desde donde comenzar a dibujar la room
            auto initRoomX = agentX;
            auto initRoomY = agentY;
            if(BordeX >= W + 1 && BordeY > H + 1){
                for(int y_room = agentY; y_room > BordeY; y_room--){
                    for(int x_room = agentX; x_room > BordeX; x_room--){
                        newMap[x_room][y_room] = 1;
                    }                
                }
            }
            else if(BordeX >= W +1){
                for(int y_room = agentY; y_room < BordeY; y_room++){
                    for(int x_room = agentX; x_room > BordeX; x_room--){
                        newMap[x_room][y_room] = 1;
                    }                
                }
            }else if (BordeY >= H + 1)
            {
                for(int y_room = agentY; y_room > BordeY; y_room--){
                    for(int x_room = agentX; x_room < BordeX; x_room++){
                        newMap[x_room][y_room] = 1;
                    }                
                }
            }
            else{
                 for(int y_room = agentY; y_room < BordeY; y_room++){
                    for(int x_room = agentX; x_room < BordeX; x_room++){
                        newMap[x_room][y_room] = 1;
                    }                
                }
            }
            
            
            //Si no se crea la room entonces incrementa la posibilidad
            
        }
        if(CreateRoom = false){
                probIncreaseRoom++;
                probabilidadRoomAux = probIncreaseRoom;
            }else{//si se crea, vuelve a ser la por defecto
                probabilidadRoomAux = probGenerateRoom;
            }
            CreateRoom = false;
    }
    std::cout << "direccion: " << direct << std::endl;
    

    // TODO: IMPLEMENTATION GOES HERE for the Drunk Agent logic.
    // The agent should move randomly.
    // You'll need a random number generator.
    // Consider:
    // - How the agent moves (possible steps).
    // - What it does if it encounters a border or an obstacle (if applicable).
    // - How it modifies the map (e.g., leaving a trail, creating rooms, etc.).
    // - Use the provided parameters (J, I, roomSizeX, roomSizeY, probabilities)
    //   to control its behavior.
    
    return newMap;
}


int main() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);
    std::cout << "--- CELLULAR AUTOMATA AND DRUNK AGENT SIMULATION ---" << std::endl;

    // --- Initial Map Configuration ---
    //int mapRows = 10;
    //int mapCols = 20;
    int mapRows = 20;
    int mapCols = 10;
    //Map myMap(mapRows, std::vector<int>(mapCols, 0)); // Map initialized with zeros
    Map myMap(mapCols, std::vector<int>(mapRows, 0));
    std::uniform_int_distribution<int> distribution_W(0, mapCols);
    std::uniform_int_distribution<int> distribution_H(0, mapRows);

    // TODO: IMPLEMENTATION GOES HERE: Initialize the map with some pattern or initial state.
    // For example, you might set some cells to 1 for the cellular automata
    // or place the drunk agent at a specific position.

    // Drunk Agent's initial position
    //int drunkAgentX = mapRows / 2;
    //int drunkAgentY = mapCols / 2;
    int drunkAgentX = distribution_H(generator);
    int drunkAgentY = distribution_W(generator);
    // If your agent modifies the map at start, you could do it here:
    // myMap[drunkAgentX][drunkAgentY] = 2; // Assuming '2' represents the agent

    std::cout << "\nInitial map state:" << std::endl;
    printMap(myMap);

    // --- Simulation Parameters ---
    int numIterations = 5; // Number of simulation steps

    // Cellular Automata Parameters
    int ca_W = mapCols;
    int ca_H = mapRows;
    int ca_R = 1;      // Radius of neighbor window
    double ca_U = 0.5; // Threshold

    // Drunk Agent Parameters
    int da_W = mapCols;
    int da_H = mapRows;
    int da_J = 10;      // Number of "walks"
    int da_I = 5;     // Steps per walk
    int da_roomSizeX = 3;
    int da_roomSizeY = 3;
    double da_probGenerateRoom = 0.1;
    double da_probIncreaseRoom = 0.05;
    double da_probChangeDirection = 0.2;
    double da_probIncreaseChange = 0.03;

    
    // --- Main Simulation Loop ---
    for (int iteration = 0; iteration < 1; ++iteration) {
        std::cout << "\n--- Iteration " << iteration + 1 << " ---" << std::endl;

        // TODO: IMPLEMENTATION GOES HERE: Call the Cellular Automata and/or Drunk Agent functions.
        // The order of calls will depend on how you want them to interact.

        // Example: First the cellular automata, then the agent
        myMap = cellularAutomata(myMap, ca_W, ca_H, ca_R, ca_U);
        //std::cout << "coor Y: " << drunkAgentY << ", coor X: " << drunkAgentX << std::endl;
        /*myMap = drunkAgent(myMap, da_W, da_H, da_J, da_I, da_roomSizeX, da_roomSizeY,
                           da_probGenerateRoom, da_probIncreaseRoom,
                           da_probChangeDirection, da_probIncreaseChange,
                           drunkAgentX, drunkAgentY);*/

        printMap(myMap);

        // You can add a delay to visualize the simulation step by step
        // #include <thread> // For std::this_thread::sleep_for
        // #include <chrono> // For std::chrono::milliseconds
        // std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    std::cout << "\n--- Simulation Finished ---" << std::endl;
    return 0;
}