import javax.swing.*;

public class RouterNode {
  private int myID;
  private GuiTextArea myGUI;
  private RouterSimulator sim;
  private int[] costs = new int[RouterSimulator.NUM_NODES];
  private int[][] distancesVectorTable =
              new int[RouterSimulator.NUM_NODES][RouterSimulator.NUM_NODES];
  private int[] route = new int[RouterSimulator.NUM_NODES];
  private int[] neighbors;

  private boolean poissonReverse = true;

  //-------------------------------------------------- Step 1
  public RouterNode(int ID, RouterSimulator sim, int[] costs) {
    myID = ID;
    this.sim = sim;
    myGUI =new GuiTextArea("  Output window for Router #"+ ID + "  ");
    int numNeighbors = 0;

    for(int i=0; i<costs.length; i++){
      if (i != myID && costs[i] != RouterSimulator.INFINITY){
        numNeighbors++;
      }
    }

    neighbors = new int[numNeighbors];
    int k = 0;
    for(int i=0; i<costs.length;i++){
      if (i != myID && costs[i] != RouterSimulator.INFINITY){
        neighbors[k++]=i;
      }
    }

    for(int i=0; i<RouterSimulator.NUM_NODES; i++){
      if (costs[i] == RouterSimulator.INFINITY){
        route[i] = RouterSimulator.INFINITY;
      }else{
        route[i] = i;
      }
    }


    System.arraycopy(costs, 0, this.costs, 0, RouterSimulator.NUM_NODES);
    for(int i=0; i<RouterSimulator.NUM_NODES; i++){
      for(int j=0; j<RouterSimulator.NUM_NODES; j++){
        if(i == myID){
          distancesVectorTable[i][j] = costs[j];
        }else{
          distancesVectorTable[i][j] = RouterSimulator.INFINITY;
        }
      }
    }

    for(int i=0;i<neighbors.length;i++){
      RouterPacket sendPacket = new RouterPacket(myID,neighbors[i]
                              ,distancesVectorTable[myID]);
      sendUpdate(sendPacket);
    }

  }

  //-------------------------------------------------- Step 1
  public void recvUpdate(RouterPacket pkt) {
    if (pkt.destid != myID){
      return;
    }

    System.arraycopy(pkt.mincost, 0, distancesVectorTable[pkt.sourceid], 0
                      , RouterSimulator.NUM_NODES);
    recalcDistanceVectorTable();
  }


  private void recalcDistanceVectorTable(){
    boolean distanceChanged = false;
    int[] routeCosts = new int[neighbors.length];
    for(int node=0;node<RouterSimulator.NUM_NODES;node++){

      if(node == myID){
        continue;
      }
      int bestCost = RouterSimulator.INFINITY;
      int bestRoute = RouterSimulator.INFINITY;
      for(int i=0;i<neighbors.length;i++){
          int tmp = costs[neighbors[i]] +
                        distancesVectorTable[neighbors[i]][node];
          if(tmp < bestCost){
            bestCost = tmp;
            bestRoute = neighbors[i];
          }
      }
      if (bestCost != distancesVectorTable[myID][node] || bestRoute != route[node]){
        distanceChanged = true;
        distancesVectorTable[myID][node] = bestCost;
        route[node] = bestRoute;
      }
    }
    if(distanceChanged){
      for(int neighborIndex=0;neighborIndex<neighbors.length;neighborIndex++){
        int[] sentDistances = new int[RouterSimulator.NUM_NODES];
        System.arraycopy(distancesVectorTable[myID], 0, sentDistances, 0
                          , RouterSimulator.NUM_NODES);

        if (poissonReverse){
          for(int j=0;j<RouterSimulator.NUM_NODES;j++){
            if (route[j] == neighbors[neighborIndex] && j != neighbors[neighborIndex]){
              sentDistances[j] = RouterSimulator.INFINITY;
            }
          }
        }
        RouterPacket sendPacket = new RouterPacket(myID,neighbors[neighborIndex]
                                ,sentDistances);
        sendUpdate(sendPacket);
      }
    }
  }

  //--------------------------------------------------
  private void sendUpdate(RouterPacket pkt) {
    sim.toLayer2(pkt);

  }


  //-------------------------------------------------- Step 1
  public void printDistanceTable() {
	  myGUI.println("Current table for " + myID +
			"  at time " + sim.getClocktime());

    String toPrint;

    myGUI.println("\nDistancetable:");
    printHeader();

    for(int i = 0; i<neighbors.length; i++){
      toPrint = " nbr\t" + neighbors[i] + "\t|";
      for(int j=0; j<RouterSimulator.NUM_NODES; j++){
        toPrint += "\t" + distancesVectorTable[neighbors[i]][j];
      }
      myGUI.println(toPrint);
    }

    myGUI.println("\nOur distance vector and routes:");
    printHeader();
    toPrint = "\tcost\t|";
    for(int i=0; i<RouterSimulator.NUM_NODES; i++){
      toPrint += "\t" + distancesVectorTable[myID][i];
    }
    myGUI.println(toPrint);

    toPrint = "\troute\t|";
    for(int i=0; i<RouterSimulator.NUM_NODES; i++){
      if(route[i] == RouterSimulator.INFINITY){
        toPrint += "\t-";
      }else{
        toPrint += "\t" + route[i];
      }
    }
    myGUI.println(toPrint);
  }

//---------------help fuction to clean up print code -----------
  private void printHeader() {
    String toPrint;
    toPrint = "\tdst\t|";
    for(int i=0; i<RouterSimulator.NUM_NODES; i++){
      toPrint += "\t" + i;
    }
    myGUI.println(toPrint);
    toPrint = printLine(45 + 23*RouterSimulator.NUM_NODES);
    myGUI.println(toPrint);

  }
  //---------help to print -----------------

  private String printLine(int num){
    String toPrint = "";
    for(int i=0; i<num; i++){
      toPrint += "-";
    }
    return toPrint;
  }

  //--------------------------------------------------
  public void updateLinkCost(int dest, int newcost) {
    costs[dest] = newcost;
    recalcDistanceVectorTable();
  }

}
