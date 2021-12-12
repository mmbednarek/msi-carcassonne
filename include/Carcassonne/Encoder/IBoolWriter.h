#ifndef MSI_CARCASSONNE_IBOOLWRITER_H
#define MSI_CARCASSONNE_IBOOLWRITER_H

namespace carcassonne::encoder {

class IBoolWriter {
 public:
   virtual void write_bool(bool value) = 0;
};

}

#endif//MSI_CARCASSONNE_IBOOLWRITER_H
