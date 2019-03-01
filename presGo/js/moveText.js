let isDragging = false;

document.addEventListener('mousedown', function(event) {
  let dragElement = event.target.closest('.draggable');

  if (!dragElement) return;
  if(dragElement.style.left == '')
    dragElement.style.left = dragElement.offsetLeft + 'px';
  if(dragElement.style.top == '')
    dragElement.style.top = dragElement.offsetTop + 'px';
  dragElement.style.right = '';
  rotate(dragElement);
  event.preventDefault();
  
  dragElement.ondragstart = function() {
      return false;
  };

  let coords, lastX, lastY;

  startDrag(dragElement, event.clientX, event.clientY);

  function onMouseUp(event) {
    finishDrag(event.clientX, event.clientY);
  };
  
  function onMouseMove(event) {
    moveAt(event.clientX, event.clientY);
    lastY = event.clientY;
    lastX = event.clientX;
  }

  // on drag start:
  //   remember the initial shift
  //   move the element position:fixed and a direct child of body
  function startDrag(element, clientX, clientY) {
    if(isDragging) {
      return;
    }
    
    isDragging = true;
    
    document.addEventListener('mousemove', onMouseMove);
    document.addEventListener('mouseup', onMouseUp);

    lastX = clientX;
    lastY = clientY;
    moveAt(clientX, clientY);
  };

  // switch to absolute coordinates at the end, to fix the element in the document
  function finishDrag(clientX, clientY) {
    if(!isDragging) {
      return;
    }
    
    isDragging = false;

    //dragElement.style.top = parseInt(dragElement.style.top) + pageYOffset + 'px';
    //dragElement.style.position = 'absolute';

    document.removeEventListener('mousemove', onMouseMove);
    document.removeEventListener('mouseup', onMouseUp);
  }

  function rotate(element)
  {
  	if(element.getAttribute("rotation") == '90')
  		return;
  	let d = 90;
    var s = "rotate(" + d + "deg)";
    if (element.style) { // regular DOM Object
        element.style.MozTransform = s
        element.style.WebkitTransform = s;
        element.style.OTransform = s;
        element.style.transform = s;
    } else if (dragElement.css) { // JQuery Object
        element.css("-moz-transform", s);
        element.css("-webkit-transform", s);
        element.css("-0-transform", s);
        element.css("transform", s);
    }
    element.setAttribute("rotation", d);
  }

  function moveAt(clientX, clientY) {
    let matchScale=/translate\(-?\d*%\s*,\s*-?\d*%\)\sscale\((-?\d*.?\d+)\)/;
    let scaleString = dragElement.offsetParent.style['transform'];
    let scale = scaleString.match(matchScale)[1];

    let newX = parseInt(dragElement.style.left) + (clientX - lastX) * (1 / parseFloat(scale));
    let newY = parseInt(dragElement.style.top) + (clientY - lastY) * (1 / parseFloat(scale));
    
	let limit = document.getElementsByClassName('limit')[0];
	
	if(newX + (dragElement.offsetWidth / 2.) < limit.offsetLeft + 25)
		newX = limit.offsetLeft - (dragElement.offsetWidth / 2.);
	if(newX + (dragElement.offsetWidth / 2.) > limit.offsetLeft + limit.offsetWidth - 25)
		newX = limit.offsetLeft + limit.offsetWidth - (dragElement.offsetWidth / 2.) - 25;

	if(newY < 100)
		newY = 100 ;
	if(newY + (dragElement.offsetWidth / 2.) > limit.offsetTop - 10)
		newY = limit.offsetTop - dragElement.offsetWidth / 2. - 10;

    dragElement.style.left =  newX + 'px';
   
    dragElement.style.top =  newY + 'px';
    
  }

});