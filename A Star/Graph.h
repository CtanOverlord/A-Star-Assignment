#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <queue>

using namespace std;

template <class NodeType, class ArcType> class GraphArc;
template <class NodeType, class ArcType> class GraphNode;

// ----------------------------------------------------------------
//  Name:           Graph
//  Description:    This is the graph class, it contains all the
//                  nodes.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
class Graph {
private:

    // typedef the classes to make our lives easier.
    typedef GraphArc<NodeType, ArcType> Arc;
    typedef GraphNode<NodeType, ArcType> Node;

// ----------------------------------------------------------------
//  Description:    An array of all the nodes in the graph.
// ----------------------------------------------------------------
    Node** m_pNodes;



// ----------------------------------------------------------------
//  Description:    The maximum number of nodes in the graph.
// ----------------------------------------------------------------
    int m_maxNodes;


// ----------------------------------------------------------------
//  Description:    The actual number of nodes in the graph.
// ----------------------------------------------------------------
    int m_count;





public:           
    // Constructor and destructor functions
    Graph( int size );
    ~Graph();

    // Accessors
    Node** nodeArray() const {
       return m_pNodes;
    }

	Node* targetNode;

	int size() const {
		return m_count;
	}

    // Public member functions.
    bool addNode( NodeType data, int index );
    void removeNode( int index );
    bool addArc( int from, int to, ArcType weight );
	void addPos( int node, int x, int y );
    void removeArc( int from, int to );
    Arc* getArc( int from, int to );        
    void clearMarks();
    void depthFirst( Node* pNode, void (*pProcess)(Node*) );
    void breadthFirst( Node* pNode, void (*pProcess)(Node*) );
	void AdaptedBreadthFirst( Node* pNode, void (*pProcess)(Node*), Node* data );
	void ucs( Node* pStart, Node* pDest, void (*pVisitFunc)(Node*), std::vector<Node *>& path);
	void aStar( Node* pStart, Node* pDest, void (*pProcess)(Node*),std::vector<Node *>& path);
	int ucs2( Node* pStart, Node* pDest, void (*pVisitFunc)(Node*));
};

// ----------------------------------------------------------------
//  Name:           Graph
//  Description:    Constructor, this constructs an empty graph
//  Arguments:      The maximum number of nodes.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
Graph<NodeType, ArcType>::Graph( int size ) : m_maxNodes( size ) {
   int i;
   m_pNodes = new Node * [m_maxNodes];
   // go through every index and clear it to null (0)
   for( i = 0; i < m_maxNodes; i++ ) {
        m_pNodes[i] = 0;
   }

   // set the node count to 0.
   m_count = 0;
}

// ----------------------------------------------------------------
//  Name:           ~Graph
//  Description:    destructor, This deletes every node
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
Graph<NodeType, ArcType>::~Graph() {
   int index;
   for( index = 0; index < m_maxNodes; index++ ) {
        if( m_pNodes[index] != 0 ) {
            delete m_pNodes[index];
        }
   }
   // Delete the actual array
   delete m_pNodes;
}

// ----------------------------------------------------------------
//  Name:           addNode
//  Description:    This adds a node at a given index in the graph.
//  Arguments:      The first parameter is the data to store in the node.
//                  The second parameter is the index to store the node.
//  Return Value:   true if successful
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
bool Graph<NodeType, ArcType>::addNode( NodeType data, int index ) {
   bool nodeNotPresent = false;
   // find out if a node does not exist at that index.
   if ( m_pNodes[index] == 0) {
      nodeNotPresent = true;
      // create a new node, put the data in it, and unmark it.
      m_pNodes[index] = new Node();
      m_pNodes[index]->setData(data);
      m_pNodes[index]->setMarked(false);
	  m_pNodes[index]->setRemoved(false);
	  m_pNodes[index]->setHueristic(0);

      // increase the count and return success.
      m_count++;
    }
        
    return nodeNotPresent;
}

// ----------------------------------------------------------------
//  Name:           removeNode
//  Description:    This removes a node from the graph
//  Arguments:      The index of the node to return.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::removeNode( int index ) {
     // Only proceed if node does exist.
     if( m_pNodes[index] != 0 ) {
         // now find every arc that points to the node that
         // is being removed and remove it.
         int node;
         Arc* arc;

         // loop through every node
         for( node = 0; node < m_maxNodes; node++ ) {
              // if the node is valid...
              if( m_pNodes[node] != 0 ) {
                  // see if the node has an arc pointing to the current node.
                  arc = m_pNodes[node]->getArc( m_pNodes[index] );
              }
              // if it has an arc pointing to the current node, then
              // remove the arc.
              if( arc != 0 ) {
                  removeArc( node, index );
              }
         }
        

        // now that every arc pointing to the current node has been removed,
        // the node can be deleted.
        delete m_pNodes[index];
        m_pNodes[index] = 0;
        m_count--;
    }
}

// ----------------------------------------------------------------
//  Name:           addArd
//  Description:    Adds an arc from the first index to the 
//                  second index with the specified weight.
//  Arguments:      The first argument is the originating node index
//                  The second argument is the ending node index
//                  The third argument is the weight of the arc
//  Return Value:   true on success.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
bool Graph<NodeType, ArcType>::addArc( int from, int to, ArcType weight ) {
     bool proceed = true; 
     // make sure both nodes exist.
     if( m_pNodes[from] == 0 || m_pNodes[to] == 0 ) {
         proceed = false;
     }
        
     // if an arc already exists we should not proceed
     if( m_pNodes[from]->getArc( m_pNodes[to] ) != 0 ) {
         proceed = false;
     }

     if (proceed == true) {
        // add the arc to the "from" node.

		float distance = sqrt(((m_pNodes[to]->getX() - m_pNodes[from]->getX()) * (m_pNodes[to]->getX() - m_pNodes[from]->getX())) + ((m_pNodes[to]->getY() - m_pNodes[from]->getY()) * (m_pNodes[to]->getY() - m_pNodes[from]->getY())));
		m_pNodes[from]->addArc( m_pNodes[to], distance );
     }
        
     return proceed;
}

template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::addPos( int node, int x, int y ) 
{
	 m_pNodes[node]->setX(x);
	 m_pNodes[node]->setY(y);
}

// ----------------------------------------------------------------
//  Name:           removeArc
//  Description:    This removes the arc from the first index to the second index
//  Arguments:      The first parameter is the originating node index.
//                  The second parameter is the ending node index.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::removeArc( int from, int to ) {
     // Make sure that the node exists before trying to remove
     // an arc from it.
     bool nodeExists = true;
     if( m_pNodes[from] == 0 || m_pNodes[to] == 0 ) {
         nodeExists = false;
     }

     if (nodeExists == true) {
        // remove the arc.
        m_pNodes[from]->removeArc( m_pNodes[to] );
     }
}


// ----------------------------------------------------------------
//  Name:           getArc
//  Description:    Gets a pointer to an arc from the first index
//                  to the second index.
//  Arguments:      The first parameter is the originating node index.
//                  The second parameter is the ending node index.
//  Return Value:   pointer to the arc, or 0 if it doesn't exist.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
// Dev-CPP doesn't like Arc* as the (typedef'd) return type?
GraphArc<NodeType, ArcType>* Graph<NodeType, ArcType>::getArc( int from, int to ) {
     Arc* pArc = 0;
     // make sure the to and from nodes exist
     if( m_pNodes[from] != 0 && m_pNodes[to] != 0 ) {
         pArc = m_pNodes[from]->getArc( m_pNodes[to] );
     }
                
     return pArc;
}


// ----------------------------------------------------------------
//  Name:           clearMarks
//  Description:    This clears every mark on every node.
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::clearMarks() {
     int index;
     for( index = 0; index < m_maxNodes; index++ ) {
          if( m_pNodes[index] != 0 ) {
              m_pNodes[index]->setMarked(false);
          }
     }
}


// ----------------------------------------------------------------
//  Name:           depthFirst
//  Description:    Performs a depth-first traversal on the specified 
//                  node.
//  Arguments:      The first argument is the starting node
//                  The second argument is the processing function.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::depthFirst( Node* pNode, void (*pProcess)(Node*) ) {
     if( pNode != 0 ) {
           // process the current node and mark it
           pProcess( pNode );
           pNode->setMarked(true);

           // go through each connecting node
           list<Arc>::iterator iter = pNode->arcList().begin();
           list<Arc>::iterator endIter = pNode->arcList().end();
        
		   for( ; iter != endIter; ++iter) {
			    // process the linked node if it isn't already marked.
                if ( (*iter).node()->marked() == false ) {
                   depthFirst( (*iter).node(), pProcess);
                }            
           }
     }
}


// ----------------------------------------------------------------
//  Name:           breadthFirst
//  Description:    Performs a depth-first traversal the starting node
//                  specified as an input parameter.
//  Arguments:      The first parameter is the starting node
//                  The second parameter is the processing function.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::breadthFirst( Node* pNode, void (*pProcess)(Node*) ) {
   if( pNode != 0 ) {
	  queue<Node*> nodeQueue;        
	  // place the first node on the queue, and mark it.
      nodeQueue.push( pNode );
      pNode->setMarked(true);

      // loop through the queue while there are nodes in it.
      while( nodeQueue.size() != 0 ) {
         // process the node at the front of the queue.
         pProcess( nodeQueue.front() );

         // add all of the child nodes that have not been 
         // marked into the queue
         list<Arc>::const_iterator iter = nodeQueue.front()->arcList().begin();
         list<Arc>::const_iterator endIter = nodeQueue.front()->arcList().end();
         
		 for( ; iter != endIter; iter++ ) {
              if ( (*iter).node()->marked() == false) {
				 // mark the node and add it to the queue.
                 (*iter).node()->setMarked(true);
                 nodeQueue.push( (*iter).node() );
              }
         }

         // dequeue the current node.
         nodeQueue.pop();
      }
   }  
}

template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::AdaptedBreadthFirst( Node* pNode, void (*pProcess)(Node*), Node* data) {
   bool found = false;
   if( pNode != 0 ) 
   {
	  queue<Node*> nodeQueue;        
	  // place the first node on the queue, and mark it.
      nodeQueue.push( pNode );
      pNode->setMarked(true);

      // loop through the queue while there are nodes in it.
      while( nodeQueue.size() != 0 && found == false) {
         // process the node at the front of the queue.
         pProcess( nodeQueue.front() );

         // add all of the child nodes that have not been 
         // marked into the queue
         list<Arc>::const_iterator iter = nodeQueue.front()->arcList().begin();
         list<Arc>::const_iterator endIter = nodeQueue.front()->arcList().end();

         
		 for( ; iter != endIter; iter++ ) {
              if ( (*iter).node()->marked() == false) 
			  {
				 // mark the node and add it to the queue.
                 (*iter).node()->setMarked(true);

				 (*iter).node()->setPrevious(nodeQueue.front());

				 
                 nodeQueue.push( (*iter).node() );
              }

			  if( (*iter).node()->data() == data->data())
			  {
				  targetNode = (*iter).node();
				  found = true;
				  break;
			  }
         }

         // dequeue the current node.
         nodeQueue.pop();
      }
   }  

   cout << endl << "Shortest Path = " << endl << targetNode->data() << endl;

   Node* tempNode = targetNode;

   while (tempNode->data() != pNode->data())
   {
		cout << tempNode->getPrevious()->data() << endl;
		tempNode = tempNode->getPrevious();
   }

}

template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::ucs( Node* pStart, Node* pDest, void (*pVisitFunc)(Node*), std::vector<Node *>& path )
{
	priority_queue<Node *, vector<Node *>, NodeSearchCostComparer> pq;

	pq.push(pStart);

	pDest = pDest;

	pq.top()->data().second = 0;


	pq.top()->setMarked(true);

	while(pq.size() != 0 && pq.top() != pDest)
	{
		 list<Arc>::const_iterator iter = pq.top()->arcList().begin();
         list<Arc>::const_iterator endIter = pq.top()->arcList().end();
        
		 for( ; iter != endIter; iter++)
		 {
			 if((*iter).node() != pq.top()->getPrevious())
			 {
				 int dist = (*iter).weight() + pq.top()->data().second;

				 if(dist < (*iter).node()->data().second)
				 {
					 (*iter).node()->data().second = dist;
					 (*iter).node()->setPrevious(pq.top());
				 }
				 if((*iter).node()->marked() == false)
				 {
					 pq.push((*iter).node());
					 (*iter).node()->setMarked(true);
				 }
			 }
		 }
		pq.pop();
	}

   path.push_back(pDest);

   Node* tempNode = pDest;

   while (tempNode != pStart)
   {
		path.push_back(tempNode->getPrevious());
		tempNode = tempNode->getPrevious();
   }

   tempNode = NULL;

   for(int i = 0; i < m_count; i++)
   {
		//nodeArray()[i]->setPrevious(NULL);
		nodeArray()[i]->setMarked(false);
   }
}

template<class NodeType, class ArcType>
int Graph<NodeType, ArcType>::ucs2( Node* pStart, Node* pDest, void (*pVisitFunc)(Node*))
{
	std::vector<Node*> path;

	priority_queue<Node *, vector<Node *>, NodeSearchCostComparer> pq;

	pq.push(pStart);

	pDest = pDest;

	pq.top()->data().second = 0;

	pq.top()->setMarked(true);

	while(pq.size() != 0 && pq.top() != pDest)
	{
		 list<Arc>::const_iterator iter = pq.top()->arcList().begin();
         list<Arc>::const_iterator endIter = pq.top()->arcList().end();
        
		 for( ; iter != endIter; iter++)
		 {
			 if((*iter).node() != pq.top()->getPrevious())
			 {
				 int dist = (*iter).weight() + pq.top()->data().second;

				 if(dist < (*iter).node()->data().second)
				 {
					 (*iter).node()->data().second = dist;
					 (*iter).node()->setPrevious(pq.top());
				 }
				 if((*iter).node()->marked() == false)
				 {
					 pq.push((*iter).node());
					 (*iter).node()->setMarked(true);
				 }
			 }
		 }
		pq.pop();
	}

   path.push_back(pDest);

   Node* tempNode = pDest;

   while (tempNode != pStart)
   {
		path.push_back(tempNode->getPrevious());
		tempNode = tempNode->getPrevious();
   }

   tempNode = NULL;

   int estimate = 0;
   for(int j = 0; j < path.size(); j++)
   {
	   estimate += path[j]->data().second;
   }

   for(int r = 0; r < m_count; r++)
   {
	   nodeArray()[r]->setPrevious(NULL);
	   nodeArray()[r]->setMarked(false);
	   nodeArray()[r]->data().second = INT_MAX
   }

   return estimate;
}

template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::aStar(Node* pStart, Node* pDest, void (*pProcess)(Node*), std::vector<Node *>& path)
{
	Node* start = pStart;
	Node* dest = pDest;
	priority_queue<Node *, vector<Node *>, NodeSearchCostComparer> pq;

	//for(int i = 0; i < m_count; i++)
	//{
	//	if(nodeArray()[i] != dest)
	//	{
	//		float estimate = ucs2(nodeArray()[i], pDest, pProcess);
	//		estimate = (estimate / 100) * 90;
	//		nodeArray()[i]->setHueristic(estimate);
	//	}
	//}

	for(int i = 0; i < m_count; i++)
	{
		if(nodeArray()[i] != dest)
		{
			long estimate = sqrt(((nodeArray()[i]->getX() - dest->getX()) * (nodeArray()[i]->getX() - dest->getX())) + (nodeArray()[i]->getY() - dest->getY()) * (nodeArray()[i]->getY() - dest->getY()));
			nodeArray()[i]->setHueristic(estimate);
			nodeArray()[i]->setCost(0);
		}	
	}		   										  
													  
	start->data().second = 0;						  
													  
	pq.push(start);									  
													  
	pq.top()->setMarked(true);						  
													  
	while(pq.size() != 0 && pq.top() != dest)		  
	{												  
		list<Arc>::const_iterator iter = pq.top()->arcList().begin();
        list<Arc>::const_iterator endIter = pq.top()->arcList().end();
        
		 for( ; iter != endIter; iter++)
		 {
			 if((*iter).node() != pq.top()->getPrevious())
			 {
				 if((*iter).node() != pq.top()->getPrevious() && (*iter).node()->marked() != true)
				 {
					 //float distC = pq.top()->data().second + ucs2(pq.top(),(*iter).node(), pProcess) + (*iter).node()->getHueristic();
					 int distC = pq.top()->data().second + (*iter).weight();
					 int fc = (*iter).node()->data().second;

					 if(distC < fc)
					 {
						 (*iter).node()->data().second = distC;
						 (*iter).node()->setCost(pq.top()->getCost() + (*iter).weight());
						 (*iter).node()->setPrevious(pq.top());
					 }
					 if((*iter).node()->marked() == false)
					 {
						 pq.push((*iter).node());
						 (*iter).node()->setMarked(true);
					 }
				 }
			 }
		 }
		 pq.pop();
	}

	Node* tempNode = pDest;

	//cout << pDest->data().first << endl;
	path.push_back(pDest);

	pDest->setRemoved(true);

	while (tempNode->getPrevious() != 0)
	{
		tempNode = tempNode->getPrevious();
		path.push_back(tempNode);
		tempNode->setRemoved(true);
		//cout << tempNode->data().first << endl;
	}

	cout << "Shortest Path from: " << "'" << start->data().first << "'" << " - " << "'" << dest->data().first << "'" << endl << endl;

	for(int i = path.size() - 1; i > -1; i--)
	{
		cout << "- " << path[i]->data().first << endl;
	}

}





#include "GraphNode.h"
#include "GraphArc.h"


#endif
