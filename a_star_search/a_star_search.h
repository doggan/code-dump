#ifndef _A_STAR_SEARCH_H_
#define _A_STAR_SEARCH_H_

#include <vector>

// TODO:
//  - Optimize OpenSet/CloseSet data structures for faster search/sort
//  - Reduce memory usage / allocation counts

/**
 * @brief Templated A-Star Algorithm for performing searches
 *        on arbitrary user states.
 *
 * @details User-defined nodes should follow the interface below:
 *
    class ExampleUserNode
    {
    public:
        ExampleUserNode();
        ExampleUserNode(...);

        /// Are the two nodes the same?
        bool isSame(const ExampleUserNode& node);

        /// Returns the cost from this node to toNode.
        float getCost(const ExampleUserNode& toNode);

        /// Returns the estimated cost heuristic to the goal node.
        float getCostEstimateToGoal(const ExampleUserNode& goalNode);

        /// Gets the neighbors connected to this node.
        /// Use aStarSearch.addNeighbor(...) to add a new neighbor.
        void getNeighbors(AStarSearch<ExampleUserNode>& aStarSearch);
    };
 */
template <class UserStateT>
class AStarSearch
{
private:
    struct Node
    {
        Node()
            :g(0.0f)
            ,h(0.0f)
            ,f(0.0f)
            ,mNextNode(NULL)
        {}

        float g;
        float h;
        float f;

        Node* mNextNode;  ///< For re-constructing the path.
        UserStateT mUserState;
    };

    typedef std::vector<Node*> NodeList;

public:
    AStarSearch(const UserStateT& startState, const UserStateT& goalState);
    ~AStarSearch();

    enum PathResult
    {
        RESULT_NONE,
        RESULT_PATH_FOUND,
        RESULT_NO_PATH
    };

    /// Gets the result of the latest search.
    PathResult getPathResult() const { return mPathResult; }

    /// Gets the start state of the resultant path.
    UserStateT* getPathStartState() const;

    /**
     * @brief Gets the 'next state' of the resultant path.
     * @details Use this to traverse the entire path.
     */
    UserStateT* getPathNextState();

    /**
     * @brief Performs a single search step.
     * @return Returns true if the search is still executing.
     */
    bool searchStep();

    /**
     * @brief Performs a search.
     * @return Returns the path result of the search.
     */
    PathResult search();

    /**
     * @brief Adds a neighbor to the node evaluation list.
     * @details Call from UserStateT::getNeighbors.
     */
    void addNeighbor(const UserStateT& state);

private:
    static bool openSetCompare(const Node* i, const Node* j)
    {
        return i->f < j->f;
    }

    static bool doesSetContain(const NodeList& list, Node* node);

    Node* popFromOpenSet();
    void pushToOpenSet(Node* node);

    Node* allocateNode(const UserStateT& state);

private:
    Node* mStartNode;
    Node* mGoalNode;

    NodeList mOpenSet;
    NodeList mClosedSet;

    NodeList mNeighborNodes;

    PathResult mPathResult;
    Node* mCurrentPathResultNode;

    NodeList mTempAllocedNodes;
    int32_t mAllocCount;
};

template <class UserStateT>
AStarSearch<UserStateT>::AStarSearch(const UserStateT& startState, const UserStateT& goalState)
    :mStartNode(NULL)
    ,mGoalNode(NULL)
    ,mPathResult(RESULT_NONE)
    ,mCurrentPathResultNode(NULL)
    ,mAllocCount(0)
{
    mStartNode = allocateNode(startState);
    mGoalNode = allocateNode(goalState);

    mStartNode->g = 0.0f;
    mStartNode->h = mStartNode->mUserState.getCostEstimateToGoal(mGoalNode->mUserState);
    mStartNode->f = mStartNode->g + mStartNode->h;

    mOpenSet.push_back(mStartNode);
}

template <class UserStateT>
AStarSearch<UserStateT>::~AStarSearch()
{
    // printf("AllocCount Before:%i, %i\n", mAllocCount, mTempAllocedNodes.size());
    /*
    for (size_t i = 0; i < mOpenSet.size(); i++)
    {
        --mAllocCount;
        delete mOpenSet[i];
    }

    for (size_t i = 0; i < mClosedSet.size(); i++)
    {
        --mAllocCount;
        delete mClosedSet[i];
    }
    */

    for (size_t i = 0; i < mTempAllocedNodes.size(); i++)
    {
        --mAllocCount;
        delete mTempAllocedNodes[i];
    }

    // printf("AllocCount Before:%i, %i\n", mAllocCount, mTempAllocedNodes.size());
    assert(mAllocCount == 0);
}

template <class UserStateT>
UserStateT* AStarSearch<UserStateT>::getPathStartState() const
{
    assert(mPathResult == RESULT_PATH_FOUND);

    return &(mStartNode->mUserState);
}

template <class UserStateT>
UserStateT* AStarSearch<UserStateT>::getPathNextState()
{
    assert(mPathResult == RESULT_PATH_FOUND);

    Node* result = mCurrentPathResultNode;

    // Finished traversing.
    if (result == NULL)
        return NULL;

    // Advance to next.
    mCurrentPathResultNode = mCurrentPathResultNode->mNextNode;

    return &(result->mUserState);
}

template <class UserStateT>
bool AStarSearch<UserStateT>::searchStep()
{
    if (mPathResult != RESULT_NONE)
    {
        // std::cout << "Search already completed." << std::endl;
        return false;
    }

    // std::cout << std::endl;
    // std::cout << "Step..." << std::endl;

    if (mOpenSet.empty())
    {
        // std::cout << "Finished - no path.";

        mPathResult = RESULT_NO_PATH;
        return false;
    }

    Node* currentNode = popFromOpenSet();
    // std::cout << "Popping: " << std::endl;
    currentNode->mUserState.dump();

    if (currentNode->mUserState.isSame(mGoalNode->mUserState))
    {
        // Copy pointer over to the goal node.
        mGoalNode->mNextNode = currentNode->mNextNode;

        // std::cout << "Goal found!" << std::endl;

        // Currently, all Node::mNextNode point to their parent nodes.
        // This allows us to traverse from goal to start.
        // For path reconstruction, we will want to traverse in the
        // opposite direction, so we swap the pointers here.
        {
            Node* prevNode = NULL;
            Node* nowNode = mGoalNode;
            while (nowNode->mNextNode != NULL)
            {
                Node* nextNode = nowNode->mNextNode;
                nowNode->mNextNode = prevNode;
                prevNode = nowNode;
                nowNode = nextNode;
            }

            // Don't forget to set the last nodes connection.
            nowNode->mNextNode = prevNode;

            assert(nowNode == mStartNode);

            // Prepare for path retrieval.
            mCurrentPathResultNode = mStartNode->mNextNode;
        }

        mPathResult = RESULT_PATH_FOUND;
        return false;
    }
    else
    {
        mClosedSet.push_back(currentNode);

        mNeighborNodes.clear();
        currentNode->mUserState.getNeighbors(*this);

        // std::cout << mNeighborNodes.size() << " neighbor nodes." << std::endl;

        for (size_t i = 0; i < mNeighborNodes.size(); i++)
        {
            Node* neighborNode = mNeighborNodes[i];

            // Already evaluated.
            if (doesSetContain(mClosedSet, neighborNode))
                continue;

            float g = currentNode->g + currentNode->mUserState.getCost(neighborNode->mUserState);
            bool isInOpenSet = doesSetContain(mOpenSet, neighborNode);
            if (!isInOpenSet || g < neighborNode->g)
            {
                neighborNode->g = g;
                neighborNode->h = neighborNode->mUserState.getCostEstimateToGoal(mGoalNode->mUserState);
                neighborNode->f = neighborNode->g + neighborNode->h;
                neighborNode->mNextNode = currentNode;

                if (!isInOpenSet)
                {
                    // std::cout << "Add to openset:" << std::endl;
                    neighborNode->mUserState.dump();
                    pushToOpenSet(neighborNode);
                }
            }
        }
    }

    // Still searching.
    return true;
}

template <class UserStateT>
typename AStarSearch<UserStateT>::PathResult AStarSearch<UserStateT>::search()
{
    while (searchStep())
    {
        // Searching...
    }

    return mPathResult;
}

template <class UserStateT>
void AStarSearch<UserStateT>::addNeighbor(const UserStateT& state)
{
    Node* node = allocateNode(state);
    mNeighborNodes.push_back(node);
}

template <class UserStateT>
typename AStarSearch<UserStateT>::Node* AStarSearch<UserStateT>::popFromOpenSet()
{
    assert(!mOpenSet.empty());

    typename NodeList::iterator it = std::min_element(mOpenSet.begin(), mOpenSet.end(), openSetCompare);
    Node* node = *it;
    mOpenSet.erase(it);

    return node;
}

template <class UserStateT>
void AStarSearch<UserStateT>::pushToOpenSet(Node* node)
{
    mOpenSet.push_back(node);
}

template <class UserStateT>
bool AStarSearch<UserStateT>::doesSetContain(const NodeList& list, Node* node)
{
    for (size_t i = 0; i < list.size(); i++)
    {
        if (list[i]->mUserState.isSame(node->mUserState))
            return true;
    }

    return false;
}

template <class UserStateT>
typename AStarSearch<UserStateT>::Node* AStarSearch<UserStateT>::allocateNode(const UserStateT& state)
{
    ++mAllocCount;

    Node* node = new Node();

    mTempAllocedNodes.push_back(node);

    node->mUserState = state;
    return node;
}

#endif // _A_STAR_SEARCH_H_
