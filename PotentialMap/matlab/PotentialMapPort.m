function PotentialMap(numZSlices, numXYSlices, axisSize, Radius, inputVoltage)
    if nargin ~= 5
        disp('Params');
        disp('<number of z axis slices>');
        disp('<number of xy slices (pixel size)>');
        disp('<axis size in mm>');
        disp('<radius of poissor in mm>');
        disp('<input voltage (kV)>');
        return;
    end
    
    z_slices = numZSlices;
    xy_slices = numXYSlices;
    axis_max = axisSize;
    radius = Radius;
    kV = inputVoltage * 1000;

    % Find out what each increment in position means (accounting for measurements in mm and not meters).
    z_space = (axis_max / z_slices) / 1000;
    xy_space = (axis_max / xy_slices) / 1000;

    xy_half = floor(xy_slices / 2);

    minPotential = inf;
    maxPotential = -inf;

    z_pos = 0.0;

    for z = (z_slices / 2) + 1:z_slices
        x_pos = 0.0;

        top = z_slices - z;
        bottom = z;

        fprintf('Calculating slice %d of %d\n', z - (z_slices / 2), z_slices / 2);

        topName = sprintf('slice%d.png', top);
        bottomName = sprintf('slice%d.png', z_slices - top);

        img = zeros(xy_slices, xy_slices, 3, 'uint8');

        for x = 1:xy_half + mod(xy_half, 2)
            y_pos = 0.0;

            for y = 1:xy_half + mod(xy_half, 2)
                potential = calcPotentialAtPoint(x_pos, y_pos, z_pos, radius, kV);
                minPotential = min(minPotential, potential);
                maxPotential = max(maxPotential, potential);

                color = colourise(potential, kV);

                imgIndexX1 = xy_half + x;
                imgIndexY1 = xy_half + y;

                imgIndexX2 = xy_half - x;
                imgIndexY2 = xy_half - y;

                % Check if indices are within the valid range
                if imgIndexX1 >= 1 && imgIndexX1 <= xy_slices && imgIndexY1 >= 1 && imgIndexY1 <= xy_slices
                    img(imgIndexX1, imgIndexY1, :) = color;
                end

                if imgIndexX2 >= 1 && imgIndexX2 <= xy_slices && imgIndexY1 >= 1 && imgIndexY1 <= xy_slices
                    img(imgIndexX2, imgIndexY1, :) = color;
                end

                if imgIndexX1 >= 1 && imgIndexX1 <= xy_slices && imgIndexY2 >= 1 && imgIndexY2 <= xy_slices
                    img(imgIndexX1, imgIndexY2, :) = color;
                end

                if imgIndexX2 >= 1 && imgIndexX2 <= xy_slices && imgIndexY2 >= 1 && imgIndexY2 <= xy_slices
                    img(imgIndexX2, imgIndexY2, :) = color;
                end

                y_pos = y_pos + xy_space;
            end

            x_pos = x_pos + xy_space;
        end

        imwrite(img, topName, 'png');

        if top ~= bottom
            imwrite(img, bottomName, 'png');
        end

        z_pos = z_pos + z_space;

        % Copy image, lay grid over it
        gridImg = img;

        % Apply grid
        for x = 1:xy_slices
            for y = 1:xy_slices
                if mod(x, 10) == 0 || mod(y, 10) == 0
                    gridImg(x, y, :) = [0, 0, 0];
                end
            end
        end

        info = sprintf('z slices: %d\nxy slices: %d\naxis size: %d\nradius: %d\nvoltage: %d\n', z_slices, xy_slices, axis_max, radius, kV);

        gridName = sprintf('grid%d.png', top);
        imwrite(gridImg, gridName, 'png');
    end

    fprintf('Min = %f\n', minPotential);
    fprintf('Max = %f\n', maxPotential);
end

function potential = calcPotentialAtPoint(x, y, z, radius, kV)
    ns_dist = calcDist(radius, x, y, z);
    ew_dist = calcDist(radius, y, z, x);
    lat_dist = calcDist(radius, z, x, y);
    
    min_dist = min([ns_dist, ew_dist, lat_dist]);
    potential = kV / min_dist;
end

function dist = calcDist(r, x, y, z)
    P = [x; y; z];
    A = getArcPos(P, r);
    D = A - P;
    
    dist = sqrt(D(1)^2 + D(2)^2 + D(3)^2);
end

function A = getArcPos(P, radius)
    A = getPosition(P(1), P(2), 0.0);
    s = toScalar(A);
    
    if s ~= 0
        A = (radius / 1000) * (A / s);
    end
end

function v = getPosition(x, y, z)
    v = [x; y; z];
end

function scalar = toScalar(v)
    scalar = sqrt(v(1)^2 + v(2)^2 + v(3)^2);
end

function colour = colourise(magnitude, limit)
    normalised = magnitude / limit;

    sensitivity = 0.1;

    if normalised > sensitivity
        colour = HlsToRgb(normalised, 0.5, 0.5);
    else
        colour = [0, 0, 0];
    end
end

function rgb = HlsToRgb(h, l, s)
    p2 = l * (1 + s);
    
    if l <= 0.5
        p1 = 2 * l;
    else
        p1 = 2 * l - p2;
    end
    
    if s == 0
        rgb = [l, l, l];
    else
        rgb = [QqhToRgb(p1, p2, h + 120), QqhToRgb(p1, p2, h), QqhToRgb(p1, p2, h - 120)];
    end
end

function rgb = QqhToRgb(q1, q2, hue)
    if hue > 360
        hue = hue - 360;
    elseif hue < 0
        hue = hue + 360;
    end
    
    if hue < 60
        rgb = q1 + (q2 - q1) * hue / 60;
    elseif hue < 180
        rgb = q2;
    elseif hue < 240
        rgb = q1 + (q2 - q1) * (240 - hue) / 60;
    else
        rgb = q1;
    end
end