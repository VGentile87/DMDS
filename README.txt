
dm2root - project dedicated to data exchange between the scanning laboratories using root data format

The library libDMRoot containes definitions for all basic structures:

1. DMRClusters   - 2-dimensional cluster object with position and shape information
2. DMRGrain      - 3-dimensional grain object with position and shape information
3. DMRMicrotrack - microtrack object
4. DMRImageCl    - image object associated with cluster
5. DMRFrame      - image object associated with frame

Expected usage is the following:
- for each scanning software should be prepared the own converter for scanned data writing 
  into the root tree using the above structures
- the resulting root files may be processed and analised by the common scrips or 
  programs on any operation system where root is installed (Linux, Windows, etc)

Application "dmrun" in src/appl/examples shows how the writing part of the interface works.
In this example file run.dm.root with a tree Vdmr inside is created
Each entry of this tree containes data for one microscope view (i.e. volume scanned by vertical movement when X,Y are fixed).

In each view we keep several arrays corresponding to different tree branches:
 cl - array of DMRCluster objects
 gr - array of DMRGrain
 mt - array of DMRMicrotrack
 im -  array of DMRImageCl
 fr - array of DMRFrame

There is also a branch for a view header
 hd - with the general information about this view (coordinates, etc)

The advantage of this structure that it's relatively simple and convenient for interactive analysis using root - all 
variables are directly accessible from the root command line (as for example: Vdmr->Draw("cl.npx") )

Relations between objects located in different branches implemented using indexes. 
The converters should take care about correct indexes filling.

In the script src/macros/check_image.C is the example of access to the image information.

Note that the images can be stored or not depending on the necessity.
This is valid in general for any branches and fields - if some information 
is not available it's not necessary to fill it with dummy values - the writing 
part will work normally anyway. Of cause if the essential information is missing 
the analysis may become limited, so it's better to provide the complete data.

== VT 21/06/2016 ==
