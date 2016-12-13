// function to get position on sphere from angles
proc vector getPosOnSphere(float $r, float $theta, float $phi) {
  
  vector $pos = <<  $r*cos($theta)*sin($phi),
                    $r*sin($theta)*sin($phi),
                    $r*cos($phi) >>;
  return $pos;
}

createNode FurrySystemNode -n furry;
connectAttr time1.outTime furry.time;

float $radius = 1.0;
int $nrOfCurves = 10;
int $currentStrandIndex = 0;
float $angleIterTheta = 2*3.14 / $nrOfCurves;
float $angleIterPhi = 3.14 / $nrOfCurves;

// loop around z-axis
for( $i=0; $i<$nrOfCurves; ++$i )
{
  // loop over sphere
  for ( $j=0; $j<$nrOfCurves; ++$j)
  {
    //get current index
    $currentStrandIndex = $j + $nrOfCurves*$i;

    // create output curve
    $outC = ( "outputcurve" + $currentStrandIndex ) ;
    createNode nurbsCurve -n $outC;
    connectAttr furry.output_curves[$currentStrandIndex] ($outC+".create");
    
    // get startPos for strand on sphere
    vector $startPos = getPosOnSphere($radius, $i*$angleIterTheta, $j*$angleIterPhi);
    
    // ugly solution, but can't operate on the variables when setting curve
    float $x = $startPos.x;
    float $y = $startPos.y;
    float $z = $startPos.z;

    float $x1 = $startPos.x*2.0;
    float $y1 = $startPos.y*2.0;
    float $z1 = $startPos.z*2.0;

    float $x2 = $startPos.x*3.0;
    float $y2 = $startPos.y*3.0;
    float $z2 = $startPos.z*3.0;

    float $x3 = $startPos.x*4.0;
    float $y3 = $startPos.y*4.0;
    float $z3 = $startPos.z*4.0;

    //Create a curve and connect it to the hair system as input shape
    curve -p $x $y $z -p $x1 $y1 $z1 -p $x2 $y2 $z2 -p $x3 $y3 $z3;
  
    string $s = ("curveShape" + ($currentStrandIndex+1) + ".intermediateObject");
    setAttr $s 1;
  
    string $c = ( "curveShape" + ($currentStrandIndex+1) ) ;
    connectAttr ($c + ".local") furry.input_curves[$currentStrandIndex];
  }

};