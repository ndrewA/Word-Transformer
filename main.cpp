#include <iostream>

#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <vector>
#include <string>
#include <memory>

struct Node
{
    std::string word;
    std::unordered_set<std::string> neighbours;
    Node(std::string w) : word(w) {}
};

class Words
{
    private:
        std::unordered_map<std::string, std::shared_ptr<Node>> nodes;
    public:
        Words(const std::unordered_set<std::string>& dictionary);
        ~Words() {}
        std::string transformWords(const std::string& startWord, const std::string& targetWord);

};

Words::Words(const std::unordered_set<std::string>& dictionary)
{
    for(const auto& word : dictionary)
    {
        for(size_t wordIter = 0; wordIter < word.size(); wordIter++)
        {
            std::shared_ptr<Node> curNode;

            if(nodes.find(word) == nodes.end())
            {
                curNode = std::make_shared<Node>(word);
                nodes.emplace(word, curNode);

            } else {

                curNode = nodes[word];
            }
            std::string iterWord = word;

            for(char charIter = 'A'; charIter <= 'Z'; charIter++)
            {
                iterWord[wordIter] = charIter;

                if(dictionary.find(iterWord) != dictionary.end() && iterWord != word)
                {
                    std::shared_ptr<Node> neighbour;

                    if(nodes.find(iterWord) == nodes.end()) {

                        neighbour = std::make_shared<Node>(iterWord);
                        nodes.emplace(iterWord, neighbour);

                    } else {

                        neighbour = nodes[iterWord];
                    }
                    curNode->neighbours.insert(iterWord);
                }
            }
        }
    }
}

std::string Words::transformWords(const std::string& startWord, const std::string& targetWord)
{
    std::queue<std::string> openStart;
    openStart.push(startWord);

    std::queue<std::string> openTarget;
    openTarget.push(targetWord);

    std::unordered_set<std::string> closedStart;
    std::unordered_set<std::string> closedTarget;

    while(!openStart.empty() && !openTarget.empty())
    {
        std::string startWord = openStart.front();

        if(closedStart.find(startWord) != closedStart.end())
        {
            openStart.pop();
            continue;
        }
        std::string curTarget = openTarget.front();

        if(closedTarget.find(curTarget) != closedTarget.end())
        {
            openTarget.pop();
            continue;
        }
        std::cout << "startWord = " << startWord << " curTarget = " << curTarget << '\n';

        openStart.pop();
        openTarget.pop();

        closedStart.insert(startWord);
        closedTarget.insert(curTarget);

        if(closedStart.find(curTarget) != closedStart.end() ||
           closedTarget.find(startWord) != closedTarget.end()) return targetWord;

        for(const auto& nextWord : nodes[startWord]->neighbours)
            openStart.push(nextWord);

        for(const auto& nextWord : nodes[curTarget]->neighbours)
            openTarget.push(nextWord);
    }
    return "-";
}

int main()
{
    Words words({{"DAMP"}, {"LAMP"}, {"LIMP"}, {"LIME"}, {"LIKE"}});
    std::string res = words.transformWords("DAMP", "LIKE");
    std::cout << "res = " << res << '\n';
    return 0;
}
