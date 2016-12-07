# MEL, NOT PYTHON

// declare a 3D vector
  vector $vec = <<5.2, 1.9, 0.3>>;

proc vector getPosOnSphere(float r, float $theta, float $phi) {
  
  vector $pos = <<  r*cos($theta)*sin($phi),
                    r*sin($theta)*sin($phi),
                    r*cos($phi) >>;
  return $pos;
}

createNode FurrySystemNode -n furry;
connectAttr time1.outTime furry.time;
//Create the output shape node of the entire hair system and connect it to the hair system

float $radius = 1.0;
int $nrOfCurves = 10;
int $angleIterTheta = 2*3.14 / $nrOfCurves;
int $angleIterPhi = 3.14 / $nrOfCurves;
//string $nameArray[] = {outputcurve1, outputcurve2};
//string $outC = "outputcurve";

// loop around z-axis
for( $i=0; $i<$nrOfCurves; ++$i )
{
  // loop over sphere
  for ( $j=0; $j<$nrOfCurves; ++$j)
  {
    // create output curve
    $outC = ( "outputcurve" + $i ) ;
    print $outC;
    createNode nurbsCurve -n $outC;
    
    connectAttr furry.output_curves[$i] ($outC+".create");
    
    vector $startPos = getPosOnSphere(radius, $i*$angleIterTheta, $j*$angleIterPhi);
    //Create a start curve and connect it to the hair system as input shape
    curve -p $startPos -p 5 0 0 -p 5 0 5 -p 0 0 5;
  
    string $s = ("curveShape" + ($i+1) + ".intermediateObject");
    setAttr $s 1;
  
    string $c = ( "curveShape" + ($i+1) ) ;
    connectAttr ($c + ".local") furry.input_curves[$i];

  }

};
