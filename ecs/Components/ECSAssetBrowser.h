//
// Created by sw_306 on 2025-08-18.
//

#ifndef ECSOBJECTVIEW_H
#define ECSOBJECTVIEW_H
#define STARTING_OFFSETX 5
#define STARTING_OFFSETY 5
#define ROWHEIGHT 20
#define TABWIDTH 20
#include "Components.hpp"
#include "ScrollBar.h"
#include "SystemCoordinator.h"
#include "Application.h"
#include "IconTextures.h"

struct ECSSegmentTreeNode
{
    int sum;
    int start,end;
};

class ECSAssetBrowser : public Entity {
    int rowHeight=20, tabWidth=20;
    std::vector<ECSSegmentTreeNode> segmentIndex;
    std::vector<int> segmentTree;
    FrameBuffer* frameBuffer;
    int selectedRow = -1, clickedRow;
    int totalrows = 0;
    float* offset;
    int* maxScroll;
    EntityID rootNode;

    void initSegmentTree(EntityID treeNode) {
        static int idx = 0;
        int nodeidx = idx;
        ECSSegmentTreeNode node{};
        node.start = idx++;
        for (auto& p : SystemCoordinator::getInstance()->GetComponent<ContentComponent>(treeNode).entities)
        {
            initSegmentTree(p);
        }
        node.end = idx-1;
        segmentIndex[nodeidx+1] = node;
    }

    int buildTree(int start, int end, int node) {
        if (start == end) {
            //return segmentTree[node] = (nodes[start-1]->expanded ? nodes[start-1]->children.size() : 0);
            return segmentTree[node] = 1;
        }
        int mid = (start+end)>>1;
        return segmentTree[node] = buildTree(start, mid, node*2) + buildTree(mid+1, end, node*2+1);
    }

    void updateTree(int start, int end, int node, int indexStart, int indexEnd, int diff) {
        if (start > end || end < indexStart || start > indexEnd) return;

        if (start == end) {
            segmentTree[node] += diff;
            return;
        }
        int mid = (start+end)>>1;
        updateTree(start, mid, node*2, indexStart, indexEnd, diff);
        updateTree(mid+1, end, node*2+1, indexStart, indexEnd, diff);
        segmentTree[node] = segmentTree[node*2] + segmentTree[node*2+1];
    }

    int sum(int start, int end, int node, int indexStart, int indexEnd) {
        if (start > indexEnd || end < indexStart) return 0;
        if (indexStart <= start && indexEnd >= end) return segmentTree[node];
        int ans = 0;
        int mid = (start+end)>>1;
        ans += sum(start, mid, node*2, indexStart, indexEnd);
        ans += sum(mid+1, end, node*2+1, indexStart, indexEnd);
        return ans;
    }

    void clicknode(EntityID node)
    {

    }
public:
    ECSAssetBrowser(glm::vec2 position, glm::vec2 size, Application* app) {
        Initialize(position, size, app);

    }

    ~ECSAssetBrowser() {
        if (frameBuffer) delete frameBuffer;
        if (offset != nullptr) delete offset;
    }

    void Initialize(glm::vec2 position, glm::vec2 size, Application* app) {
        ScrollBar* scrollbar = new ScrollBar();
        scrollbar->Initialize(position, size);
        SystemCoordinator::getInstance()->RegisterEntity(this);
        SystemCoordinator::getInstance()->AddComponent(getId(), TransformComponent{position,glm::vec4(40,40,40,1), size});
        SystemCoordinator::getInstance()->AddComponent(getId(), ScrollableComponent{0,0,0,0, scrollbar->getId()});
        SystemCoordinator::getInstance()->AddComponent(getId(), ContentComponent{});

        offset = &SystemCoordinator::getInstance()->GetComponent<ScrollableComponent>(getId()).offset;
        maxScroll = &SystemCoordinator::getInstance()->GetComponent<ScrollableComponent>(getId()).maxScroll;
        auto& entities = SystemCoordinator::getInstance()->GetComponent<ContentComponent>(getId()).entities;

        rootNode = CreateTreeNode(getId());
        SystemCoordinator::getInstance()->GetComponent<TreeNodeComponent>(rootNode).visible = true;
        SystemCoordinator::getInstance()->GetComponent<RenderableIcon>(rootNode).visible = true;
        loadTree(&app->getRoot(),rootNode,position.x+STARTING_OFFSETX,position.y+STARTING_OFFSETY, entities);

        int entitysize = entities.size();
        segmentIndex.resize(entitysize);
        segmentTree.resize(entitysize*4);
        segmentIndex.push_back({0,0,0});
        initSegmentTree(rootNode);
        buildTree(1, entitysize-1, 1);
        updateTree(0,entities);

        //updateTree(0, entities);
        for (auto& p : entities) {
            bool visible = SystemCoordinator::getInstance()->GetComponent<TreeNodeComponent>(p).visible;
            std::string text = SystemCoordinator::getInstance()->GetComponent<TextComponent>(p).text;
            std::cout << text << ' ' << visible << std::endl;
        }
    }

    void setIcon(SceneNode* node, NodeComponent* node_component) {
        if
    }

    void loadTree(SceneNode* sceneNode, EntityID node, int width, int height, std::vector<EntityID>& entities, NodeComponent* node_component = nullptr) {
        SystemCoordinator::getInstance()->GetComponent<TextComponent>(node).text = sceneNode->getName();
        SystemCoordinator::getInstance()->GetComponent<PositionComponent>(node).position = glm::vec2(width,height);
        if (node_component) {
            SystemCoordinator::getInstance()->GetComponent<TextComponent>(node).text = node_component->getType();
        }

        auto& boundingbox = SystemCoordinator::getInstance()->GetComponent<ClickableComponent>(node).boundingBox;
        auto transform = SystemCoordinator::getInstance()->GetComponent<TransformComponent>(getId());
        boundingbox = glm::vec4(glm::vec2(transform.position.x,height), boundingbox.z, boundingbox.w);
        auto& icon = SystemCoordinator::getInstance()->GetComponent<RenderableIcon>(node);
        SystemCoordinator::getInstance()->GetComponent<NonRenderableBoundingBox>(node).boundingBox = glm::vec4(transform.position.x, height, transform.size.x, rowHeight);
        icon.boundingBox = glm::vec4(width,height,tabWidth,rowHeight);
        icon.renderRect = icon.boundingBox+glm::vec4(1,1,-1,-1);
        entities.reserve(entities.size() + 1);
        entities.emplace_back(node);


        for (int i=0,j=0; ; i++) {
            if (node_component) break;

            if (i < sceneNode->getChildren().size()) {
                EntityID child = CreateTreeNode(node);
                loadTree(sceneNode->getChildren()[i], child, width+tabWidth, height+rowHeight*(i+1), entities);
                SystemCoordinator::getInstance()->GetComponent<ContentComponent>(node).entities.emplace_back(child);
            }
            else if (sceneNode->getComponents().size() && j < sceneNode->getComponents().size()) {
                EntityID child = CreateTreeNode(node);
                loadTree(sceneNode, child, width+tabWidth, height+rowHeight*(i+1), entities, sceneNode->getComponents()[j]);
                SystemCoordinator::getInstance()->GetComponent<ContentComponent>(node).entities.emplace_back(child);
                j++;
            }
            else break;


        }
        // for (; i<sceneNode->getComponents().size(); i++)
        // {
        //     EntityID child = CreateTreeNode(node);
        //     loadTree(sceneNode, child, width+tabWidth, height+rowHeight*(i+1), entities, sceneNode->getComponents()[i]);
        //     //EntityID child = CreateTreeNode(node, typeid(sceneNode->getComponents()[i]).name(), glm::vec2(width+tabWidth,height + rowHeight*(i+1)),glm::vec4(transform.position.x, height + rowHeight*(i+1), transform.size.x, rowHeight));
        //     SystemCoordinator::getInstance()->GetComponent<ContentComponent>(node).entities.emplace_back(child);
        //     //entities.reserve(entities.size() + 1);
        //     //entities.emplace_back(child);
        // }
    }

    void updateTree(int idx, std::vector<EntityID>& nodes) {
        if (idx+1 >= nodes.size() || idx < 0) return;
        int cnt = 0;
        //updateTree(1, nodes.size()-1, idx+1, segmentIndex[idx].start, segmentIndex[idx].end, nodes[idx]->expanded ? -segmentTree[idx+1] : segmentTree[idx+1]);
        int sum1 = sum(1,nodes.size()-1, 1, segmentIndex[idx+1].start, segmentIndex[idx+1].end);
        auto& nodeidx = SystemCoordinator::getInstance()->GetComponent<TreeNodeComponent>(nodes[idx]);
        for (int i = idx+1; i<=idx+sum1; i++)
        {
            TreeNodeComponent* nodei = &SystemCoordinator::getInstance()->GetComponent<TreeNodeComponent>(nodes[i]);
            RenderableIcon* nodeicon = &SystemCoordinator::getInstance()->GetComponent<RenderableIcon>(nodes[i]);
            TreeNodeComponent* nodeiparent = &SystemCoordinator::getInstance()->GetComponent<TreeNodeComponent>(SystemCoordinator::getInstance()->GetComponent<ParentComponent>(nodes[i]).parent);
            if (nodeidx.expanded)
            {
                if (!nodeiparent->expanded)
                {
                    i+=sum(1,nodes.size()-1,1,segmentIndex[i+1].start, segmentIndex[i+1].end);
                    nodei = &SystemCoordinator::getInstance()->GetComponent<TreeNodeComponent>(nodes[i]);
                    nodeiparent = &SystemCoordinator::getInstance()->GetComponent<TreeNodeComponent>(SystemCoordinator::getInstance()->GetComponent<ParentComponent>(nodes[i]).parent);
                }
                if (nodeiparent->expanded)
                {
                    nodei->visible = nodeiparent->expanded & nodeidx.expanded;
                    //nodes[i]->icon.visible = nodes[i]->parent->expanded & nodes[idx]->expanded;
                    if (SystemCoordinator::getInstance()->GetComponent<ContentComponent>(nodes[i]).entities.size()) {
                        nodeicon->visible = nodeiparent->expanded & nodeidx.expanded;
                    }

                    cnt++;
                }
            }
            else
            {
                if (nodei->visible)
                {
                    nodei->visible = false;
                    //nodes[i]->icon.visible = false;
                    nodeicon->visible = false;
                    cnt++;
                }
            }
        }
        for (int i = idx+sum1; i<nodes.size(); i++)
        {
            auto& nodei = SystemCoordinator::getInstance()->GetComponent<PositionComponent>(nodes[i]);
            if (idx) {
                nodei.position.y += (cnt-1)*rowHeight * (nodeidx.expanded ? 1 : -1);
                SystemCoordinator::getInstance()->GetComponent<ClickableComponent>(nodes[i]).boundingBox.y += (cnt-1)*rowHeight * (nodeidx.expanded ? 1 : -1);
                auto& icon = SystemCoordinator::getInstance()->GetComponent<RenderableIcon>(nodes[i]);
            }
            //icon.boundingBox += cnt*rowHeight * (nodeidx.expanded ? 1 : -1);
            //icon.renderRect += cnt*rowHeight * (nodeidx.expanded ? 1 : -1);
            //nodes[i]->icon.position.y += cnt*rowHeight * (nodeidx.expanded ? 1 : -1);
        }
        std::cout << "sum: " << sum1 << std::endl;
        std::cout << "cnt: " << cnt << std::endl;
        totalrows += cnt*(nodeidx.expanded ? 1 : -1);
        *maxScroll = totalrows * rowHeight + STARTING_OFFSETY - SystemCoordinator::getInstance()->GetComponent<TransformComponent>(getId()).size.y;
    }

    EntityID CreateTreeNode(EntityID parent = -1, std::string text="", glm::vec2 pos=glm::vec2(), glm::vec4 boundingBox=glm::vec4()) {
        EntityID id = SystemCoordinator::getInstance()->CreateEntity();
        SystemCoordinator::getInstance()->AddComponent(id, TextComponent{std::move(text)});
        SystemCoordinator::getInstance()->AddComponent(id, PositionComponent{pos});
        SystemCoordinator::getInstance()->AddComponent(id, TreeNodeComponent{0,0,0,-1,getId()});
        SystemCoordinator::getInstance()->AddComponent(id, ContentComponent{});
        SystemCoordinator::getInstance()->AddComponent(id, ParentComponent{parent});
        SystemCoordinator::getInstance()->AddComponent(id, NonRenderableBoundingBox{boundingBox});
        SystemCoordinator::getInstance()->AddComponent(id, RenderableIcon{EXPAND_ARROW, glm::vec4(pos,glm::vec2(tabWidth,rowHeight)), glm::vec4(pos+glm::vec2(1,1), glm::vec2(tabWidth,rowHeight)-glm::vec2(1,1))});
        //temporary implementation please fix at later date
        SystemCoordinator::getInstance()->AddComponent(id, ClickableComponent{glm::vec4(pos,glm::vec2(SystemCoordinator::getInstance()->GetComponent<TransformComponent>(getId()).size.x, rowHeight)), [this](EntityID entity)
        {
            auto& treenode = SystemCoordinator::getInstance()->GetComponent<TreeNodeComponent>(entity);
            auto& uvrect = SystemCoordinator::getInstance()->GetComponent<RenderableIcon>(entity).uvRect;
            int childrensize = SystemCoordinator::getInstance()->GetComponent<ContentComponent>(entity).entities.size();
            if (!treenode.visible) return;
            glm::vec4 iconBoundingBox = SystemCoordinator::getInstance()->GetComponent<RenderableIcon>(entity).boundingBox;
            glm::vec4 nodeBoundingBox = SystemCoordinator::getInstance()->GetComponent<ClickableComponent>(entity).boundingBox;
            glm::vec2 mousePos = Input::getInstance()->getMousePos();
            iconBoundingBox.y -= *offset;
            nodeBoundingBox.y -= *offset;
            if (iconBoundingBox.x <= mousePos.x && mousePos.x <= iconBoundingBox.x + iconBoundingBox.z && iconBoundingBox.y <= mousePos.y && mousePos.y <= iconBoundingBox.y + iconBoundingBox.w)
            {
                if (!childrensize) return;
                treenode.expanded = !treenode.expanded;
                uvrect = treenode.expanded ? EXPANDED_ARROW : EXPAND_ARROW;
                std::cout << "haha lcikc" << std::endl;
                selectedRow = entity;
                SystemCoordinator::getInstance()->AddComponent(getId(), DirtyComponent{});
            }
            else if (nodeBoundingBox.y <= mousePos.y && mousePos.y <= nodeBoundingBox.y+nodeBoundingBox.w)
            {
                SystemCoordinator::getInstance()->GetComponent<TreeNodeComponent>(clickedRow).selected = false;
                clickedRow = entity;
                treenode.selected = true;
            }
        }});
        return id;
    }

    void update(float deltatime) override {
        updateTree(selectedRow-rootNode, SystemCoordinator::getInstance()->GetComponent<ContentComponent>(getId()).entities);
    }

    int getTotalRows() {
        return totalrows;
    }


};

#endif //ECSOBJECTVIEW_H
