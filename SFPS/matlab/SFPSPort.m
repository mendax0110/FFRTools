classdef SFPS
    properties
        mass
        charge
        time_step
        velocity
    end
    
    methods
        function obj = SFPS()
            obj.mass = 1.67262158e-27;  % Mass of a proton in kg
            obj.charge = 1.602e-19;     % Charge of a proton in C
            obj.time_step = 1e-8;       % Time step in seconds
            obj.velocity = Vector3d(0, 0, 0);
        end
        
        function newPos = move(obj, currentPosition, electricField)
            force = obj.charge * electricField;
            acceleration = force / obj.mass;
            
            displacement = obj.velocity * obj.time_step + 0.5 * acceleration * obj.time_step^2;
            newPos = currentPosition + displacement;
        end
    end
end

classdef Vector3d
    properties
        x
        y
        z
    end
    
    methods
        function obj = Vector3d(px, py, pz)
            obj.x = px;
            obj.y = py;
            obj.z = pz;
        end
    end
end

classdef ParticleModel
    properties
        lastPos
        particle
        track
    end
    
    methods
        function obj = ParticleModel(start)
            obj.lastPos = start;
            obj.particle = Proton();
            obj.track = start;
        end
        
        function newPos = move(obj, electricField)
            delta = obj.particle.move(obj.lastPos, electricField);
            obj.lastPos = obj.lastPos + delta;
            obj.track = [obj.track; obj.lastPos];
            newPos = obj.lastPos;
        end
        
        function track = getTrack(obj)
            track = obj.track;
        end
    end
end

function simulateParticles(config)
    field = MappedFieldModel(config.mapSize);

    centreSize = [2.0, 2.0, 2.0];
    collision = CollisionModel(config.mapSize, centreSize);

    map = PointMap(config.voltage);
    field.createFieldMap(map);

    startPositions = RandomPositionFactory(config.mapSize);

    for i = 1:config.particles
        start = startPositions();
        particle = ParticleModel(start);
        
        whileMoving(start, particle, collision, field, config.maxLength);

        for j = 1:length(particle.getTrack())
            disp(particle.getTrack(j));
        end

        disp('(end)');
    end
end

function whileMoving(start, particle, collision, field, maxLength)
    currentPos = start;

    for num = 1:maxLength
        v = field(currentPos);

        if collision(currentPos)
            break;
        end

        currentPos = particle.move(v);
    end
end

classdef MappedFieldModel
    properties
        scale
        size
        xMax
        yMax
        zMax
        forceMap
    end
    
    methods
        function obj = MappedFieldModel(mapSize)
            obj.scale = 1;
            obj.size = [mapSize(1)/obj.scale, mapSize(2)/obj.scale, mapSize(3)/obj.scale];
            obj.xMax = floor(obj.size(1));
            obj.yMax = floor(obj.size(2));
            obj.zMax = floor(obj.size(3));
        end
        
        function createFieldMap(obj, map)
            mapSize = obj.xMax * obj.yMax * obj.zMax;
            assert(mapSize < ((250*1024*1024) / 8));  % Assuming field_t is a double (8 bytes)

            obj.forceMap = zeros(1, mapSize);

            for x = 1:obj.xMax
                for y = 1:obj.yMax
                    for z = 1:obj.zMax
                        point = [x, y, z];
                        force = map(point);
                        obj.forceMap(obj.getMapPosition(x, y, z)) = force;
                    end
                end
            end
        end
        
        function force = subsref(obj, point)
            force = obj.forceMap(obj.getMapPosition(point));
        end
    end
    
    methods (Access = private)
        function pos = getMapPosition(obj, x, y, z)
            px = x * obj.yMax * obj.zMax;
            py = y * obj.zMax;

            pos = px + py + z;
        end
    end
end

classdef PointMap
    properties
        potential
    end
    
    methods
        function obj = PointMap(p)
            obj.potential = p;
        end
        
        function E = subsref(obj, p)
            E = VElectricField.from(p, obj.potential);
        end
    end
end

classdef RandomPositionFactory
    properties
        size
        seed
        genX
        genY
        genZ
        vX
        vY
        vZ
    end
    
    methods
        function obj = RandomPositionFactory(mapSize)
            obj.size = mapSize;
            obj.seed = RandStream('mt19937ar', 'Seed', sum(100*clock));
            obj.genX = UniformOutputRandStream(obj.seed, 0, obj.size(1));
            obj.genY = UniformOutputRandStream(obj.seed, 0, obj.size(2));
            obj.genZ = UniformOutputRandStream(obj.seed, 0, obj.size(3));
            obj.vX = obj.genX;
            obj.vY = obj.genY;
            obj.vZ = obj.genZ;
        end
        
        function pos = subsref(obj, ~)
            pos = [obj.vX(), obj.vY(), obj.vZ()];
        end
    end
end

classdef CollisionModel
    properties
        area
        centre
    end
    
    methods
        function obj = CollisionModel(area, centre)
            obj.area = area;
            obj.centre = centre;
        end
        
        function result = subsref(obj, p)
            result = (p(1) >= obj.area(1) || p(2) >= obj.area(2) || p(3) >= obj.area(3) || ...
                      p(1) <= obj.centre(1) || p(2) <= obj.centre(2) || p(3) <= obj.centre(3));
        end
    end
end

% Assuming you have already defined VElectricField, VPoint, Velocity, Acceleration, Force, and other necessary classes

% Example usage:
config = struct('mapSize', [10, 10, 10], 'voltage', 1.0, 'particles', 5, 'maxLength', 100);
simulateParticles(config);
