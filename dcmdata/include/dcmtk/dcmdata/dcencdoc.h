/*
 *
 *  Copyright (C) 2018, OFFIS e.V.
 *  All rights reserved.  See COPYRIGHT file for details.
 *
 *  This software and supporting documentation were developed by
 *
 *    OFFIS e.V.
 *    R&D Division Health
 *    Escherweg 2
 *    D-26121 Oldenburg, Germany
 *
 *
 *  Module:  dcmdata
 *
 *  Author:  Pedro Arizpe
 *
 *  Purpose: Class to control document encapsulation into DICOM files
 *
 */

#ifndef DCENCDOC_H
#define DCENCDOC_H
//make sure OS specific configuration is included first
#include "dcmtk/config/osconfig.h"
//for override keys
#include "dcmtk/dcmdata/dcpath.h"
#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/ofstd/ofconapp.h"
#include "dcmtk/ofstd/ofxml.h"

/** This class handles common functions of all command line applications for document encapsulation.
 */
class DCMTK_DCMDATA_EXPORT DcmEncapsulatedDocument
{
public:
  /** Constructor
   *  @param none
   */
   DcmEncapsulatedDocument();

  ///Destructor, frees plugin memory
   ~DcmEncapsulatedDocument();

  /**
   *  This function is only used to process CDA files. It retrieves all entries of an attribute and
   *  returns them separated by backslashes.
   *  @param fileNode: the root XML node.
   *  @param attr: the attribute to search for.
   *  @return OFstring containing all entries found, separated by double backslashes.
   */
  OFString XMLgetAllAttributeValues(XMLNode fileNode, OFString attr);
  /** This function is only used to process CDA files. It retrieves the value from the CDA document
   *  corresponding to the DCM Tag. According to Stantard v. 2013 part20/sect_A.8.
   *  @param fileNode: the root XML node.
   *  @param attr: the tag to search for in the CDA file.
   *  @return OFstring containing the value of the corresponding tag.
   */
  OFString XMLgetAttribute(XMLNode fileNode, DcmTagKey attr);

  /** Retrieves patient, concept and document data from the CDA file and checks for data conflicts
   *  with series, study and user input. It also retrieves all mediatypes found in the CDA document.
   *  @param filename [in] - The filename of the CDA document.
   *  @param logger [in] - The logger of the application calling this method.
   *  @return EXITCODE_NO_ERROR (0) if successfull or error code in case of failure.
   */
  int getCDAData(const char *filename, OFLogger &appLogger);

  /** Recursive function used by getAttributeValues to get all occurrences of an attribute as list.
   *  @param currnode: the current XML node to be processed.
   *  @param *results: a reference to the list of strings where the results should be stored.
   *  @param attr: the attribute to search for.
   *  @return OFTrue if the attribute value was found, OFFalse otherwise.
   */
  OFBool XMLsearchAttribute(XMLNode currnode, OFList<OFString> *results, OFString attr);

  /** Add CDA specific command line options to the OFCommandLine object
   *  passed to the constructor.
   *  @param cmd a reference to an OFCommandLine object used to parse
   *    the command line argument give to the calling application.
   *  @return none
   */
  void addCDACommandlineOptions(OFCommandLine& cmd);

  /** Add PDF specific command line options to the OFCommandLine object
   *  passed to the constructor.
   *  @param cmd a reference to an OFCommandLine object used to parse
   *    the command line argument give to the calling application.
   *  @return none
   */
  void addPDFCommandlineOptions(OFCommandLine& cmd);

  /** Add STL specific command line options to the OFCommandLine object
   *  passed to the constructor.
   *  @param cmd a reference to an OFCommandLine object used to parse
   *    the command line argument give to the calling application.
   *  @return none
   */
  void addSTLCommandlineOptions(OFCommandLine& cmd);

  /** Add general command line options to the OFCommandLine object
   *  passed to the constructor.
   *  @param cmd a reference to an OFCommandLine object used to parse
   *    the command line argument give to the calling application.
   *  @return none
   */
  void addGeneralOptions(OFCommandLine &cmd);

  /** Add command line options specific for documents to the OFCommandLine
   *  object passed to the constructor.
   *  @param cmd a reference to an OFCommandLine object used to parse
   *    the command line argument give to the calling application.
   *  @return none
   */
  void addDocumentOptions(OFCommandLine &cmd);

  /** Add command line options specific for output to the OFCommandLine
   *  object passed to the constructor.
   *  @param cmd a reference to an OFCommandLine object used to parse
   *    the command line argument give to the calling application.
   *  @return none
   */
  void addOutputOptions(OFCommandLine &cmd);

  /** Parse and evaluate the given command line arguments.
   *  @param app a reference to an OFConsoleApplication object used in the
   *    calling application.
   *  @param cmd a reference to an OFCommandLine object used to parse
   *    the command line argument give to the calling application.
   *  @return none
   */
  void parseArguments(OFConsoleApplication& app, OFCommandLine& cmd);

  /** Includes basic information into the dicom file  */
  OFCondition createHeader (DcmItem *dataset,
    OFLogger& logger,
    const char *opt_mediaTypes = "");

  /** Correctly inserts encapsulated document data.
   *  @param dataset [in] - The dataset to which we should add this.
   *  @param logger [in] - The logger of the application calling this method
   *  @return EXITCODE_NO_ERROR (0) if successfull or error code in case of failure.
   */
  int insertEncapsulatedDocument(DcmItem *dataset,
    OFLogger& logger);

  /** Get study or series data from provided file. Generate UID if none present.
   *  @param logger [in] - The logger of the application calling this method
   *  @return Error code as condition, if error occurs, EC_Normal otherwise
   */
  OFCondition createIdentifiers(OFLogger& appLogger);

  /** Copy override keys over existing keys in given dataset.
   *  @param outputDset - [out] dataset to which the override keys are copied
   *  @return Error code as condition, if error occurs, EC_Normal otherwise
   */
  OFCondition applyOverrideKeys(DcmDataset *outputDset);

  /** Specifies some attributes that should be inserted after encapsulation.
   *  They will override any identical attributes already existing in the resulting encapsulated
   *  DICOM object. The override keys are applied at the very end of the conversion and do not
   *  undergo any validity checking.
   *  @param ovkeys [in] override keys that can be tags, dictionary names and paths (see DcmPath
   *  for syntax). Also it is permitted to set a value if appropriate, e. g. "PatientName=Doe^John"
   *  would be a valid overridekey.
   *  @return none
   */
  void setOverrideKeys(const OFList<OFString>& ovkeys);

  /** Returns the input file name.
   *  @param none
   *  @return the input file name as OFString.
   */
  OFString getInputFileName();

  /** Sets the input file name to the given string.
   *  @param fName the file name to be set.
   *  @return none
   */
  void setInputFileName(OFString fName);

  /** Returns the output file name.
   *  @param none
   *  @return the output file name as OFString.
   */
  OFString getOutputFileName();

  /** Sets the output file name.
   *  @param fName the file name to be set.
   *  @return none
   */
  void setOutputFileName(OFString fName);

  /** Get study or series data from provided file. Generate UID if none present.
   *  @param logger [in] - The logger of the application calling this method
   *  @return Error code as condition, if error occurs, EC_Normal otherwise
   */
  OFCondition saveFile(DcmFileFormat fileformat);

  /** Returns the transfer syntax.
   *  @param none
   *  @return the transfer syntax.
   */
  E_TransferSyntax getTransferSyntax();

private:
  ///input file name
  OFString             opt_ifname;
  ///output file name
  OFString             opt_ofname;

  ///optional parameters
  ///*patient data
  OFString             opt_patientBirthdate;
  OFString             opt_patientID;
  OFString             opt_patientName;
  OFString             opt_patientSex;
  ///*concept data
  OFString             opt_conceptCM;
  OFString             opt_conceptCSD;
  OFString             opt_conceptCV;
  ///*document specific options
  OFString             opt_documentTitle;
  OFString             opt_seriesFile;
  OFString             opt_seriesUID;
  OFString             opt_studyUID;

  ///*assign default values for file encoding and padding
  E_EncodingType          opt_oenctype;
  E_FileWriteMode         opt_writeMode;
  E_GrpLenEncoding        opt_oglenc;
  E_PaddingEncoding       opt_opadenc;
  E_TransferSyntax        opt_oxfer;
  OFCmdUnsignedInt        opt_filepad;
  OFCmdUnsignedInt        opt_itempad;
  ///*pre-existing series
  OFBool                  opt_readSeriesInfo;
  OFBool                  opt_annotation;
  OFBool                  opt_increment;

  OFCmdSignedInt          opt_instance;
  /** These attributes are applied to the dataset after conversion
   * (They are not checked by the isValid() function)
   */
  OFList<OFString>        opt_overrideKeys;

  /// CDA specific variables
  OFString                cda_mediaTypes;
  OFBool                  opt_override;
};
#endif // DCENCDOC_H