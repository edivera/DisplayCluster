#ifndef DISPLAY_GROUP_H
#define DISPLAY_GROUP_H

#include "Marker.h"
#include <QtGui>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include "ContentWindow.h" 
class DisplayGroupGraphicsView;

enum MESSAGE_TYPE { MESSAGE_TYPE_CONTENTS, MESSAGE_TYPE_CONTENTS_DIMENSIONS, MESSAGE_TYPE_PIXELSTREAM, MESSAGE_TYPE_QUIT };

#define MESSAGE_HEADER_URI_LENGTH 64

struct MessageHeader {
    int size;
    MESSAGE_TYPE type;
    char uri[MESSAGE_HEADER_URI_LENGTH]; // optional URI related to message. needs to be a fixed size so sizeof(MessageHeader) is constant
};

class DisplayGroup : public QObject, public boost::enable_shared_from_this<DisplayGroup> {
    Q_OBJECT

    public:

        boost::shared_ptr<DisplayGroupGraphicsView> getGraphicsView();

        Marker & getMarker();

        void addContentWindow(boost::shared_ptr<ContentWindow> contentWindow);
        void removeContentWindow(boost::shared_ptr<ContentWindow> contentWindow);
        bool hasContent(std::string uri);
        void setContentWindows(std::vector<boost::shared_ptr<ContentWindow> > contentWindows);
        std::vector<boost::shared_ptr<ContentWindow> > getContentWindows();

        void moveContentWindowToFront(boost::shared_ptr<ContentWindow> contentWindow);

        void dump() 
        { 
          if (contentWindows_.size()) 
          { 
            std::cerr << "DisplayGroup: \n"; 
            for(unsigned int i=0; i<contentWindows_.size(); i++) 
            { 
              boost::shared_ptr<ContentWindow> c = contentWindows_[i]; 
              c->dump(); 
            } 
          } 
        } 


    public slots:

        void synchronize();

        void sendDisplayGroup();
        void sendContentsDimensionsRequest();
        void sendPixelStreams();
        void sendQuit();

        void advanceContents();

    private:
        friend class boost::serialization::access;

        template<class Archive>
        void serialize(Archive & ar, const unsigned int version)
        {
            ar & marker_;
            ar & contentWindows_;
        }

        // marker
        Marker marker_;

        // vector of all of its content windows
        std::vector<boost::shared_ptr<ContentWindow> > contentWindows_;

        // used for GUI display
        boost::shared_ptr<DisplayGroupGraphicsView> graphicsView_;

        void receiveDisplayGroup(MessageHeader messageHeader);
        void receiveContentsDimensionsRequest(MessageHeader messageHeader);
        void receivePixelStreams(MessageHeader messageHeader);
};

typedef boost::shared_ptr<DisplayGroup> pDisplayGroup; 
 
class pyDisplayGroup 
{ 
public: 
  pyDisplayGroup(pDisplayGroup pdg) {ptr = pdg;} 
  ~pyDisplayGroup() {} 
 
  pDisplayGroup getp() {return ptr;} 
 
  bool hasContent(const char *uri) { return ptr->hasContent(std::string(uri)); } 
  void addContentWindow(const pyContentWindow& cw) {ptr->addContentWindow(cw.get());} 
  void removeContentWindow(const pyContentWindow& cw) {ptr->removeContentWindow(cw.get());} 
  void dump() {ptr->dump();} 
   
private: 
  pDisplayGroup ptr; 
}; 
 
extern pyDisplayGroup getThePyDisplayGroup(); 

#endif
